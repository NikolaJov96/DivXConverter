#include "../subtitleio.h"

#include <QStringList>
#include <QDebug>
#include <QFile>

void MPSub::loadTitle(Subtitles& subs, const QString &path, double fps) const
{
    // load text file in MPSub format form path p
    // parse it and store to subs

    QFile inFile(path);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        throw CantOpenFile(path);

    subs.setFPS(fps);
    QTextStream inStream(&inFile);
    inStream.setCodec("UTF-8");
    QString time, data, nextLine;
    long line = 0;
    double timePoint = 0;
    QStringList list;
    while (!inStream.atEnd())
    {
        // read one block

        // read and skip blank lines and title ID
        time = "";
        while (!inStream.atEnd() &&
               (time.length() == 0 || time.startsWith("#")))
            time = inStream.readLine(), line++;
        if (inStream.atEnd() && time.length() > 0)
            throw IOException(path);

        // read and convert start and end time
        while (time.startsWith("#")) time = inStream.readLine(), line++;
        QRegExp timeRX("([^ ]+)");
        int pos = 0;
        list.clear();
        while ((pos = timeRX.indexIn(time, pos)) != -1) {
            list.append(timeRX.cap(1));
            pos += timeRX.matchedLength();
        }
        if (list.size() != 2)
            throw InvalidTimeFormat(path, line);

        double start = timePoint + list[0].toDouble() * 1000;
        double end = start + list[1].toDouble() * 1000;
        timePoint = end;

        // read title text
        data = "";
        while (!inStream.atEnd() &&
               (nextLine = inStream.readLine()).length() > 0)
        {
            line++;
            if (nextLine.startsWith("#")) continue;
            if (data.length() > 0) data.append("\n");
            data.append(nextLine);
        }
        line++;

        // add title to
        Subtitle *newTitle = new Subtitle(data, start, end);
        subs.addSubTitle(newTitle);
    }
    subs.setFileSize(inFile.size());
    inFile.close();
}

void MPSub::saveTitle(Subtitles &subs, const QString &path) const
{
    // convert titles from subs to MPSub format
    // remove <i> and <\i> flasgs
    // save it to file with path p

    QFile outFile(path);
    if (!outFile.open(QIODevice::WriteOnly))
        throw CantOpenFile(path);

    QTextStream outStream(&outFile);
    outStream.setCodec("UTF-8");
    long timePoint = 0;
    for (auto &sub : subs.getTitles())
    {
        // leave blank space
        if (timePoint > 0) outStream << endl;

        // convert and write time
        long start = sub->getStart() - timePoint;
        long len = sub->getEnd() - sub->getStart();
        timePoint += start + len;
        outStream << QString::number(start / 1000.0, 'f', 2) << " "
                  << QString::number(len / 1000.0, 'f', 2) << endl;

        // write title text
        QString data = sub->getText();
        while (data.contains("<i>")) data.replace("<i>", "");
        while (data.contains("</i>")) data.replace("</i>", "");
        outStream << data << endl;
    }
    outFile.close();
    subs.setFileSize(getFileSize(path));
}
