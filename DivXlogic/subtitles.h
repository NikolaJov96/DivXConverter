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
    ~Subtitles();

    std::vector<Subtitle*> &getTitles();                /*!< Return reference to Subtitle* vector */
    std::vector<Subtitle*> const &getTitles() const;    /*!< Return const reference to Subtitle* vector */

    double getFPS() const;                              /*!< Return current FPS setting */
    QString const &getFilePath() const;
    FORMATS getFormat() const;
    bool isEdited() const;

    void setFPS(double);                                /*!< FPS setter */
    void setFilePath(const QString&);
    void setFormat(FORMATS);
    void setEdited(bool);

    long indexOf(const QString&) const;
    void addSubTitle(Subtitle*);                        /*!< Inserts new subtitle (to the end by default)*/
private:
    std::vector<Subtitle*> subtitles;                   /*!< Vector of individual subtitles */
    double FPS = DEFAULT_FPS;                           /*!< Current FPS setting */
    QString filePath = "";                              /*!< Opened file path */
    FORMATS fileType = FORMATS::UNDEFINED;              /*!< Opened file format */
    bool edited = false;
};

#endif // SUBTITLES_H
