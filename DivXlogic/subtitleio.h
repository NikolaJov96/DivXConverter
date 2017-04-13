#ifndef SUBTITLEIO_H
#define SUBTITLEIO_H

#include <QString>

/*!
 * \brief The FORMATS enum
 *
 * Enum class containing supported subtitle
 * formats and additional UNDEFINED flag
 */
enum class FORMATS {UNDEFINED, SRT, MPSub, MicroDVD};

const double DEFAULT_FPS = 25.0;

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
    virtual void saveTitle(const Subtitles&, const QString&) const = 0;     /*!< Saves Subtitles to file with provided path */
    static FORMATS detect(const QString&);                                  /*!< Determines the format of the file with provided path */
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
    void saveTitle(const Subtitles&, const QString&) const;                 /*!< Saves Subtitles to SRT file with provided path */
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
    void loadTitle(Subtitles&, const QString&, double) const;              /*!< Loads Subtitles from MicroDVD file with provided path and FPS */
    void saveTitle(const Subtitles&, const QString&) const;                /*!< Saves Subtitles to MicroDVD file with provided path */
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
    void saveTitle(const Subtitles&, const QString&) const;                 /*!< Saves Subtitles to MPSub file with provided path */
};

#endif // SUBTITLEIO_H
