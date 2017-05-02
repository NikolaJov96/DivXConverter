#ifndef SUBTITLE_H
#define SUBTITLE_H

#include <QString>

/*!
 * \brief The Subtitle class
 *
 * Class that contains information about single subtitle
 */
class Subtitle
{
public:
    Subtitle(const QString&, long, long);   /*!< Constructor with provided text, start and end time in ms */

    QString &getText();                     /*!< Returns reference to the QString */
    QString const &getText() const;         /*!< Returns const reference to the QString */
    long getStart() const;                  /*!< Returns start time in ms */
    long getEnd() const;                    /*!< Returns end time in ms */
    QString const &getSStart() const;       /*!< Returns formated start time */
    QString const &getSEnd() const;         /*!< Returns formated end time */

    void setStart(long);               /*!< Updates subtitle start time */
    void setEnd(long);                 /*!< Updates subtitle end time */

    bool operator==(const Subtitle&) const; /*!< Compares two Subtitles */
private:
    QString text;                           /*!< Text of the subtitle */
    long start;                             /*!< Subtitle start time in ms */
    long end;                               /*!< Subtitle end time in ms */
    QString sStart;                         /*!< String reprezentation of the start time */
    QString sEnd;                           /*!< String reprezentation of the ent time */

    QString longToTime(long) const;         /*!< Returns formated timestamp */
};

#endif // SUBTITLE_H
