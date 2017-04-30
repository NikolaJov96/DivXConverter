#include "../subtitleio.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QString>

void MicroDVD::loadTitle(Subtitles &subs, const QString &path, double fps) const
{
    // load text file in MicroDVD format form path p
    // parse it and store to subs

    QFile inFile(path);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        throw CantOpenFile(path);

    subs.setFPS(fps);
    QTextStream inStream(&inFile);
    inStream.setCodec("UTF-8");
    QString row;
    QStringList list;
    QRegExp timeRX("\\{([0-9]+)\\}");
    while (!inStream.atEnd())
    {
        // read one block

        row = inStream.readLine();

        int pos = 0, pos1;
        list.clear();
        while ((pos = timeRX.indexIn(row, pos)) != -1) {
            list.append(timeRX.cap(1));
            pos1 = pos += timeRX.matchedLength();
        }
        if (list.size() != 2)
            throw InvalidTimeFormat(path, 123);

        long start = list[0].toLong() * 1000 / subs.getFPS();
        long end = list[1].toLong() * 1000 / subs.getFPS();
        QString data = row.mid(pos1).replace("|", "\n");

        // add title to
        Subtitle *newTitle = new Subtitle(data, start, end);
        subs.addSubTitle(newTitle);
    }
    inFile.close();
}

void MicroDVD::saveTitle(const Subtitles &subs, const QString &path) const
{
    // convert titles from subs to MicroDVD format
    // save it to file with path p

    QFile outFile(path);
    if (!outFile.open(QIODevice::WriteOnly))
        throw CantOpenFile(path);

    QTextStream outStream(&outFile);
    outStream.setCodec("UTF-8");
    for (auto &sub : subs.getTitles())
    {
        // convert and write time and data
        outStream << "{" << sub->getStart() * subs.getFPS() / 1000 << "}"
                  << "{" << sub->getEnd() * subs.getFPS() / 1000 << "}"
                  << sub->getText().replace("\n", "|") << "\n";
    }
    outFile.close();
}
