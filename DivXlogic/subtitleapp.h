#ifndef SUBTITLEAPP_H
#define SUBTITLEAPP_H

const double DEFAULT_FPS = 25.0;

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
    SubtitleApp();                                  /*!< Default constructor */
    ~SubtitleApp();                                 /*!< Default destructor */

    Subtitles &getSubtitles();                      /*!< Returns reference to the Subtitles container */

    bool isLoaded() const;                          /*!< Checks if subtitle file is loaded */
    QString const &getFilePath() const;             /*!< Returns reference to opened file path */

    void loadTitle(const QString&,
                   FORMATS, double = DEFAULT_FPS);  /*!< Loads subtitle file with provided path, format and FPS */
    void saveTitle(const QString&, FORMATS);        /*!< Saves to file with provided path and format */
    void saveTitle();                               /*!< Saves to file with remembered path and format */
private:
    Subtitles subtitles;                            /*!< Subtitle container */
    SRT SRTManager;                                 /*!< Object for SRT format IO */
    MPSub MPSubManager;                             /*!< Object for MPSub format IO */
    MicroDVD MicroDVDManager;                       /*!< Object for MicroDVD format IO */

    bool loaded = false;                            /*!< File opened indicator */
    QString filePath = "";                          /*!< Opened file path */
    FORMATS fileType = FORMATS::UNDEFINED;          /*!< Opened file format */

    void clearData();                               /*!< Clear all data, close opened file */
    SubtitleIO const *getIOManager(FORMATS) const;  /*!< Returns IO manager for desired format */
};

#endif // SUBTITLEAPP_H
