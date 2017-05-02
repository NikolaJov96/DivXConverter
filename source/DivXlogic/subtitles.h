#ifndef SUBTITLES_H
#define SUBTITLES_H

#include <vector>

class Subtitles;

#include "subtitle.h"
#include "subtitleio.h"

/*!
 * \brief The Subtitles class
 *
 * Class container for Subtitle instances
 */
class Subtitles
{
public:
    ~Subtitles();                                       /*!< Clearing of the Subtitle vector */

    std::vector<Subtitle*> &getTitles();                /*!< Returns reference to Subtitle* vector */
    std::vector<Subtitle*> const &getTitles() const;    /*!< Returns const reference to Subtitle* vector */

    double getFPS() const;                              /*!< Returns current FPS setting */
    QString const &getFilePath() const;                 /*!< Returns save path QString */
    FORMATS getFormat() const;                          /*!< Retunrs save file format */
    long getConsistency() const;                        /*!< Check if file is consistent */
    bool isEdited() const;                              /*!< Checks if file is edited after the last save */

    void setFPS(double);                                /*!< Sets new FPS falue */
    void setFilePath(const QString&);                   /*!< Sets new save path value */
    void setFormat(FORMATS);                            /*!< Sets new format of the save file */
    void setEdited(bool);                               /*!< Sets edited flag */
    void setConsistency(long);                          /*!< Sets consistancy flag */

    long indexOf(const QString&) const;                 /*!< Returns the index of title with provided start time */
    void addSubTitle(Subtitle*);                        /*!< Inserts new subtitle (to the end by default)*/

private:
    std::vector<Subtitle*> subtitles;                   /*!< Vector of individual subtitles */
    double FPS = DEFAULT_FPS;                           /*!< Current FPS setting */
    QString filePath = "";                              /*!< Opened file path */
    FORMATS fileType = FORMATS::UNDEFINED;              /*!< Opened file format */
    bool edited = false;                                /*!< Flag implying file is edited after last save */
    long consistent = -1;                               /*!< Number of the title that is inconsistent */
};

#endif // SUBTITLES_H
