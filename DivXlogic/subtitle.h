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
    long &getStart();                       /*!< Returns reference to the start time in ms */
    long &getEnd();                         /*!< Returns reference to the end time in ms */
    long getStart() const;                  /*!< Returns start time in ms */
    long getEnd() const;                    /*!< Returns end time in ms */
    QString getSStart() const;              /*!< Returns formated start time */
    QString getSEnd() const;                /*!< Returns formated end time */

private:
    QString text;                           /*!< Text of the subtitle */
    long start;                             /*!< Subtitle start time in ms */
    long end;                               /*!< Subtitle end time in ms */

    QString longToTime(long) const;         /*!< Returns formated timestamp */
};

#endif // SUBTITLE_H
