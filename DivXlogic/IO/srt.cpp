#include "../subtitleio.h"

#include <QFile>
#include <QTextStream>
#include <QRegExp>

SRT::SRT() {}

long SRT::timeToLong(const QString &time) const
{
    // convert string time to ms - format: 01:03:00,900
    QRegExp rx("([0-9]+)");
    int pos = 0;
    while ((pos = rx.indexIn(time, pos)) != -1);
    if (rx.captureCount() != 4) { /* throw invalid time */ }
    return ((rx.cap(3).toLong() * 60 +
             rx.cap(2).toLong()) * 60 +
            rx.cap(1).toLong()) * 1000 +
            rx.cap(0).toLong();
}

QString SRT::longToTime(long ms) const
{
    // convert ms to string time - format: 01:03:00,900
    return QString("%1").arg(ms / 1000 / 60 / 60, 2, 10, QChar('0')) + ":" +
            QString("%1").arg((ms / 1000 / 60) % 60, 2, 10, QChar('0')) + ":" +
            QString("%1").arg((ms / 1000) % 60, 2, 10, QChar('0')) + "," +
            QString("%1").arg(ms % 1000, 3, 10, QChar('0'));
}

void SRT::loadTitle(Subtitles &subs, const QString &p) const
{
    // load text file in SRT format form path p
    // parse it and store to subs

    QFile inFile(p);
    if (!inFile.isOpen())
    {
        // throw unable to open file
    }
    QTextStream inStream(&inFile);
    QString time, data, nextLine;
    while (!inStream.atEnd())
    {
        // read one block

        // read and skip blank lines and title ID
        while (!inStream.atEnd() &&
               (time = inStream.readLine()).length() == 0);
        if (inStream.atEnd() && time.length() > 0)
        { /* throw invalid file */ }

        // read and convert start and end time
        time = inStream.readLine();
        long start = timeToLong(time.mid(0, 12));
        long end  = timeToLong(time.mid(17));

        // read title text
        data = "";
        while (!inStream.atEnd() &&
               (nextLine = inStream.readLine()).length() > 0)
        {
            if (data.length() > 0) data.append("\n");
            data.append(nextLine);
        }

        // add title to
        Subtitle *newTitle = new Subtitle(data, start, end);
        subs.addSubTitle(newTitle);
    }
    inFile.close();
}

void SRT::saveTitle(Subtitles& subs, const QString&p) const
{
    // convert titles from subs to SRT format
    // save it to file with path p

    QFile outFile(p);
    if (!outFile.isOpen())
    {
        // throw unable to open file
    }
    QTextStream outStream(&outFile);
    int ID = 0;
    for (auto &sub : subs.getTitles())
    {
        // leave blank space
        if (ID > 0) outStream << "\n";

        // write title ID
        outStream << ++ID << "\n";

        // convert and write time
        outStream << longToTime(sub->getStart())
                  << " --> "
                  << longToTime(sub->getEnd()) << "\n";

        // write title text
        outStream << sub->getText() << "\n";
    }
    outFile.close();
}
