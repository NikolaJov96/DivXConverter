#include "subtitleprocessing.h"

#include <QDebug>

#include <edittitledialog.h>

SubtitleProcessing::SubtitleProcessing(Subtitles *s, QWidget *mw) :
    window(mw),
    subs(s)
{}

bool SubtitleProcessing::editTitle(long ind) const
{
    if (ind < 0 || ind >= subs->getTitles().size())
        return false;

    Subtitle temp = *(subs->getTitles()[ind]);
    editTitleDialog dialog(&temp, window);
    dialog.setModal(true);
    dialog.exec();

    // if no changes are appliyed, return
    if (temp == *(subs->getTitles()[ind])) return false;
    if (temp.getStart() > temp.getEnd()) return false;

    *(subs->getTitles()[ind]) = temp;
    std::sort(subs->getTitles().begin(),
              subs->getTitles().end(),
              [](const Subtitle *a, const Subtitle *b) -> bool {
        if (a->getStart() == b->getStart())
            return a->getEnd() < b->getEnd();
        return a->getStart() < b->getStart();
    });
    subs->setEdited(true);
    return true;
}

bool SubtitleProcessing::editTitle(const QString &start) const
{
    int ind = subs->indexOf(start);
    if (ind == -1) return false;
    return editTitle(ind);
}

bool SubtitleProcessing::addSubtitle() const
{
    Subtitle *temp = new Subtitle("", 0, 0);
    editTitleDialog dialog(temp, window);
    dialog.setModal(true);
    dialog.exec();

    if (*temp == Subtitle("", 0, 0) ||
            temp->getStart() > temp->getEnd())
    {
        delete temp;
        return false;
    }
    subs->addSubTitle(temp);
    std::sort(subs->getTitles().begin(),
              subs->getTitles().end(),
              [](const Subtitle *a, const Subtitle *b) -> bool {
        return a->getStart() < b->getStart();
    });
    subs->setEdited(true);
    return true;
}

bool SubtitleProcessing::deleteSubtitle(long ind) const
{
    if (ind < 0 || ind >= subs->getTitles().size())
        return false;

    subs->getTitles().erase(subs->getTitles().begin() + ind);
    subs->setEdited(true);
    return true;
}

bool SubtitleProcessing::deleteSubtitle(const QString &start) const
{
    int ind = subs->indexOf(start);
    if (ind == -1) return false;
    return deleteSubtitle(ind);
}

bool SubtitleProcessing::timeShift(long ind, long ms) const
{
    if (ind < 0 || ind >= subs->getTitles().size())
        return false;
    if (subs->getTitles()[ind]->getStart() + ms < 0)
        return false;
    subs->getTitles()[ind]->getStart() += ms;
    subs->getTitles()[ind]->getEnd() += ms;
    subs->setEdited(true);
    return true;
}

bool SubtitleProcessing::timeShift(const QString &start, long ms) const
{
    int ind = subs->indexOf(start);
    if (ind == -1) return false;
    return timeShift(ind, ms);
}
