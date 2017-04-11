#include "../subtitleio.h"

#include <QStringList>
#include <QDebug>
#include <QFile>

MPSub::MPSub() {}

void MPSub::loadTitle(Subtitles& subs, const QString&p, double fps) const
{
    // load text file in MPSub format form path p
    // parse it and store to subs

    QFile inFile(p);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // throw unable to open file
        qInfo() << "Unable to open file!\n" << p << "\n";
        return;
    }
    subs.setFPS(fps);
    QTextStream inStream(&inFile);
    QString time, data, nextLine;
    double timePoint = 0;
    QStringList list;
    while (!inStream.atEnd())
    {
        // read one block

        // read and skip blank lines and title ID
        time = "";
        while (!inStream.atEnd() &&
               (time.length() == 0 || time.startsWith("#")))
            time = inStream.readLine();
        if (inStream.atEnd() && time.length() > 0)
        { qInfo() << "Invalid file!\n"; /* throw invalid file */ }

        // read and convert start and end time
        while (time.startsWith("#")) time = inStream.readLine();
        QRegExp rx("([^ ]+)");
        int pos = 0;
        list.clear();
        while ((pos = rx.indexIn(time, pos)) != -1) {
            list.append(rx.cap(1));
            pos += rx.matchedLength();
        }
        if (list.size() != 2) { qInfo() << "Invalid time!\n" << time << "\n"; /* throw invalid time */ }
        double start = timePoint + list[0].toDouble() * 1000;
        double end = start + list[1].toDouble() * 1000;
        timePoint = end;

        // read title text
        data = "";
        while (!inStream.atEnd() &&
               (nextLine = inStream.readLine()).length() > 0)
        {
            if (nextLine.startsWith("#")) continue;
            if (data.length() > 0) data.append("\n");
            data.append(nextLine);
        }

        // add title to
        Subtitle *newTitle = new Subtitle(data, start, end);
        subs.addSubTitle(newTitle);
    }
    inFile.close();
}

void MPSub::saveTitle(const Subtitles& subs, const QString&p) const
{
    // convert titles from subs to MPSub format
    // remove <i> and <\i> flasgs
    // save it to file with path p

    QFile outFile(p);
    if (!outFile.open(QIODevice::WriteOnly))
    {
        qInfo() << "Unable to open file!\n" << p << "\n";
        return;
        // throw unable to save file
    }
    QTextStream outStream(&outFile);
    long timePoint = 0;
    for (auto &sub : subs.getTitles())
    {
        // leave blank space
        if (timePoint > 0) outStream << "\n";

        // convert and write time
        long start = sub->getStart() - timePoint;
        long len = sub->getEnd() - sub->getStart();
        timePoint += start + len;
        outStream << QString::number(start / 1000.0, 'f', 2) << " "
                  << QString::number(len / 1000.0, 'f', 2) << "\n";

        // write title text
        outStream << sub->getText() << "\n";
    }
    outFile.close();
}
