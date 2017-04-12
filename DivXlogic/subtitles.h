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
    double getFPS() const;                              /*!< Return current FPS setting */
    std::vector<Subtitle*> &getTitles();                /*!< Return reference to Subtitle* vector */
    std::vector<Subtitle*> const &getTitles() const;    /*!< Return const reference to Subtitle* vector */
    void setFPS(double);                                /*!< FPS setter */

    long subtitleCo() const;                            /*!< Returns number of subtitles in container */
    bool isEmpty() const;                               /*!< Checks if container is empty */
    long indexOf(const QString&) const;
    void addSubTitle(Subtitle*, long t = -1);           /*!< Inserts new subtitle (to the end by default)*/
private:
    std::vector<Subtitle*> subtitles;                   /*!< Vector of individual subtitles */
    double FPS = 25.0;                                  /*!< Current FPS setting */
    QString filePath = "";                              /*!< Opened file path */
    FORMATS fileType = FORMATS::UNDEFINED;              /*!< Opened file format */
};

#endif // SUBTITLES_H
