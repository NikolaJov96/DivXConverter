#include "../subtitleio.h"

#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QDebug>

long SRT::timeToLong(const QString &time) const
{
    // convert string time to ms - format: 01:03:00,900
    QRegExp intRX("([0-9]+)");
    long arr[4];
    int pos = 0, ind = 0;
    while ((pos = intRX.indexIn(time, pos)) != -1) {
        if (ind > 3) throw InvalidTimeFormat();
        arr[ind++] = intRX.cap(1).toLong();
        pos += intRX.matchedLength();
    }
    if (ind != 4) { throw InvalidTimeFormat(); }
    return ((arr[0] * 60 + arr[1]) * 60 + arr[2]) * 1000 + arr[3];
}

QString SRT::longToTime(long ms) const
{
    // convert ms to string time - format: 01:03:00,900
    return QString("%1").arg(ms / 1000 / 60 / 60, 2, 10, QChar('0')) + ":" +
            QString("%1").arg((ms / 1000 / 60) % 60, 2, 10, QChar('0')) + ":" +
            QString("%1").arg((ms / 1000) % 60, 2, 10, QChar('0')) + "," +
            QString("%1").arg(ms % 1000, 3, 10, QChar('0'));
}

void SRT::loadTitle(Subtitles &subs, const QString &path, double fps) const
{
    // load text file in SRT format form path p
    // parse it and store to subs

    QFile inFile(path);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        throw CantOpenFile(path);

    subs.setFPS(fps);
    QTextStream inStream(&inFile);
    inStream.setCodec("UTF-8");
    QString time, data, nextLine;
    QRegExp timeMatch ("[0-9]+:[0-9]+:[0-9]+,[0-9]+ +--> +[0-9]+:[0-9]+:[0-9]+,[0-9]+");
    while (!inStream.atEnd())
    {
        // read one block

        // read and skip blank lines and title ID
        while (!inStream.atEnd() &&
               (time = inStream.readLine()).length() == 0);
        if (inStream.atEnd() && time.length() > 0)
            throw IOException(path);

        // read and convert start and end time
        time = inStream.readLine();
        if (!timeMatch.exactMatch(time)) throw InvalidTimeFormat(path, 1234);
        long start, end;
        try
        {
            int position = time.toStdString().find("-->");
            start = timeToLong(time.mid(0, position));
            end = timeToLong(time.mid(position + 3));
        }
        catch (InvalidTimeFormat&) { throw InvalidTimeFormat(path, 123); }

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

void SRT::saveTitle(const Subtitles& subs, const QString &path) const
{
    // convert titles from subs to SRT format
    // save it to file with path p

    QFile outFile(path);
    if (!outFile.open(QIODevice::WriteOnly))
        throw CantOpenFile(path);

    QTextStream outStream(&outFile);
    outStream.setCodec("UTF-8");

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