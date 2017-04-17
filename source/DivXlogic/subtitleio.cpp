#include "subtitleio.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegExp>

IOException::IOException(const QString &p) :
    path(p) {}

QString IOException::what() const
{
    return "Error while reading file: " + path;
}

InvalidTimeFormat::InvalidTimeFormat(
        const QString &p, long l) :
    IOException(p),
    line(l) {}

QString InvalidTimeFormat::what() const
{
    return "Invalid time format in line: " + QString::number(line) + " in file: " + path;
}

CantOpenFile::CantOpenFile(const QString &p) :
    IOException(p) {}

QString CantOpenFile::what() const
{
    return "Unable to open file: " + path;
}

FORMATS SubtitleIO::detect(const QString &path)
{
    // detect subtitle file format
    // do not check if file is actualy valid

    QFile inFile(path);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        throw CantOpenFile(path);

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
