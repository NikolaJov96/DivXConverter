#ifndef SUBTITLEAPP_H
#define SUBTITLEAPP_H

#include <vector>

#include "subtitleio.h"
#include "subtitles.h"

/*!
 * \brief The SubtitleApp class
 *
 * Class that contains all of the information about
 * current state of the app, provides access to it's
 * members and initializes requested procedures
 */
class SubtitleApp
{
public:
    ~SubtitleApp();                                 /*!< Default destructor */

    Subtitles *getSubtitles(int);                   /*!< Returns Subtitles* with provided index */
    std::vector<Subtitles*> &getSubtitles();
    int getFilesCo();                               /*!< Returns number of opened Subtitles */

    void newTitle();                                /*!< Adds new blank Subtitles */
    void loadTitle(const QString&,
                   FORMATS, double = DEFAULT_FPS);  /*!< Loads subtitle file with provided path, format and FPS */
    void saveTitle(const QString&, FORMATS, int);   /*!< Saves to file with provided path and format */
    void saveTitle(int);                            /*!< Saves to file with remembered path and format */
    void closeFile(int);                            /*!< Closes opened file */
private:
    std::vector<Subtitles*> files;                  /*!< Vector of all opened files */
    SRT SRTManager;                                 /*!< Object for SRT format IO */
    MPSub MPSubManager;                             /*!< Object for MPSub format IO */
    MicroDVD MicroDVDManager;                       /*!< Object for MicroDVD format IO */

    SubtitleIO const *getIOManager(FORMATS) const;  /*!< Returns IO manager for desired format */
};

#endif // SUBTITLEAPP_H
