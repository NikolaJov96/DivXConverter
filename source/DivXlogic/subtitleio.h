#ifndef SUBTITLEIO_H
#define SUBTITLEIO_H

#include <QString>

/*!
 * \brief The IOException class
 *
 * Exception thrown for unsuccessful
 * IO operations
 */
class IOException
{
public:
    IOException(const QString&);                    /*!< Constructor with provided file path */
    virtual QString what() const;            /*!< Returns excepotion message */
protected:
    QString path;                                   /*!< File path */
};

/*!
 * \brief The InvalidTimeFormat class
 *
 * Exception thrown on invalid time
 * format while loading file
 */
class InvalidTimeFormat : public IOException
{
public:
    InvalidTimeFormat(const QString &p = "",
                      long l = -1);                 /*!< Constructor with provided file path and line*/
    QString what() const;                           /*!< Returns excepotion message */
private:
    long line;                                      /*!< Line with error */
};

/*!
 * \brief The CantOpenFile class
 *
 * Exception thrown on unsuccessful
 * opening of the file
 */
class CantOpenFile : public IOException
{
public:
    CantOpenFile(const QString&);                   /*!< Constructor with provided file path */
    QString what() const;                           /*!< Returns excepotion message */
};

/*!
 * \brief The FORMATS enum
 *
 * Enum class containing supported subtitle
 * formats and additional UNDEFINED flag
 */
enum class FORMATS {UNDEFINED, SRT, MPSub, MicroDVD};

const double DEFAULT_FPS = 25.0;        /*!< Default FPS used when not user specified */

#include "subtitles.h"

/*!
 * \brief The SubtitleIO class
 *
 * Abstract class - template for IO managers
 * for all supported formats
 */
class SubtitleIO
{
public:
    virtual void loadTitle(Subtitles&, const QString&, double) const = 0;   /*!< Loads Subtitles from file with provided path and FPS */
    virtual void saveTitle(Subtitles&, const QString&) const = 0;           /*!< Saves Subtitles to file with provided path */
    static FORMATS detect(const QString&);                                  /*!< Determines the format of the file with provided path */
    static long long getFileSize(const QString&);                           /*!< Returns size of the file with provided path  */
};

/*!
 * \brief The SRT class
 *
 * Class containig methods for IO of SRT format
 */
class SRT : public SubtitleIO
{
public:
    void loadTitle(Subtitles&, const QString&, double) const;               /*!< Loads Subtitles from SRT file with provided path and FPS */
    void saveTitle(Subtitles&, const QString&) const;                       /*!< Saves Subtitles to SRT file with provided path */
private:
    long timeToLong(const QString&) const;                                  /*!< Converts SRT timestamp format to ms */
    QString longToTime(long) const;                                         /*!< Converts ms to SRT time format */
};

/*!
 * \brief The MicroDVD class
 *
 * Class containig methods for IO of MicroDVD format
 */
class MicroDVD : public SubtitleIO
{
public:
    void loadTitle(Subtitles&, const QString&, double) const;               /*!< Loads Subtitles from MicroDVD file with provided path and FPS */
    void saveTitle(Subtitles&, const QString&) const;                       /*!< Saves Subtitles to MicroDVD file with provided path */
};

/*!
 * \brief The MPSub class
 *
 * Class containig methods for IO of MPSub format
 */
class MPSub : public SubtitleIO
{
    friend class Subtitles;
public:
    void loadTitle(Subtitles&, const QString&, double) const;               /*!< Loads Subtitles from MPSub file with provided path and FPS */
    void saveTitle(Subtitles&, const QString&) const;                       /*!< Saves Subtitles to MPSub file with provided path */
};

#endif // SUBTITLEIO_H
