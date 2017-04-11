#include "subtitleio.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegExp>

FORMATS SubtitleIO::detect(const QString &path)
{
    // detect subtitle file format
    // do not check if file is actualy valid

    QFile inFile(path);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qInfo() << "Unable to open file!\n" << path << "\n";
        return FORMATS::UNDEFINED;
    }
    QTextStream inStream(&inFile);
    QString line;
    while (!inStream.atEnd() &&
           (line = inStream.readLine()).length() == 0);
    inFile.close();
    if (line.length() == 0) return FORMATS::UNDEFINED;
    if (line.startsWith("#")) return FORMATS::MPSub;
    if (line.startsWith("{")) return FORMATS::MicroDVD;
    QRegExp re("\\d+");
    if (re.exactMatch(line)) return FORMATS::SRT;
    return FORMATS::MPSub;
}
