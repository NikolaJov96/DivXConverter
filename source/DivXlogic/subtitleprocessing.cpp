#include "subtitleprocessing.h"

#include <QDebug>

#include <edittitledialog.h>

SubtitleProcessing::SubtitleProcessing(Subtitles *s, QWidget *mw) :
    window(mw),
    subs(s)
{}

void SubtitleProcessing::sort() const
{
    std::sort(subs->getTitles().begin(),
              subs->getTitles().end(),
              [](const Subtitle *a, const Subtitle *b) -> bool {
        if (a->getStart() == b->getStart())
            return a->getEnd() < b->getEnd();
        return a->getStart() < b->getStart();
    });
}

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

bool SubtitleProcessing::autoConcat(long maxDist, long maxLen)
{
    bool edited = false;
    for (int i = 0; i < subs->getTitles().size() - 1; i++)
    {
        Subtitle *s1 = subs->getTitles()[i],
                *s2 = subs->getTitles()[i + 1];
        if (s2->getStart() - s1->getEnd() <= maxDist &&
                s1->getText().length() + s2->getText().length() <= maxLen)
        {
            s1->getEnd() = s2->getEnd();
            s1->getText() += "\n" + s2->getText();
            subs->getTitles().erase(subs->getTitles().begin() + i + 1);
            i--;
            edited = true;
        }
    }
    if (edited) subs->setEdited(true);
    return edited;
}

bool SubtitleProcessing::autoSplit(long maxDuration, long maxLen)
{
    bool edited = false;
    for (int i = 0; i < subs->getTitles().size(); i++)
    {
        Subtitle *s1 = subs->getTitles()[i];
        if (s1->getEnd() - s1->getStart() >= maxDuration &&
                s1->getText().length()>= maxLen)
        {
            QString text = s1->getText().mid(
                        s1->getText().length() / 2);
            if (text.startsWith("\n")) text = text.mid(1, text.length() - 1);
            long start = s1->getStart() +
                    (s1->getEnd() - s1->getStart()) / 2 - 1;
            Subtitle *s2 = new Subtitle(text, start, s1->getEnd());
            s1->getEnd() = start - 2;
            s1->getText() = s1->getText().mid(0, s1->getText().length() / 2);
            if (s1->getText().endsWith("\n"))
                s1->getText() = s1->getText().mid(0, s1->getText().length() - 1);
            subs->getTitles().insert(
                        subs->getTitles().begin() + i + 1,
                        s2);
            i++;
            edited = true;
        }
    }
    if (edited) subs->setEdited(true);
    return edited;
}

bool SubtitleProcessing::appendFile(const Subtitles &append)
{
    long delta = 0;
    if (!subs->getTitles().empty())
        delta = subs->getTitles()[
                subs->getTitles().size() - 1]->getEnd();
    for (auto sub : append.getTitles())
        subs->addSubTitle(new Subtitle(
                              sub->getText(),
                              sub->getStart() + delta,
                              sub->getEnd() + delta));
    subs->setEdited(true);
    return true;
}

long SubtitleProcessing::isConsistent() const
{
    for (int i = 0; i < subs->getTitles().size() - 1; i++)
        if (subs->getTitles()[i]->getEnd() > subs->getTitles()[i+1]->getStart())
            return i;
    return -1;
}
