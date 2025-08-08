/*
 *  Quackle -- Crossword game artificial intelligence and analysis tool
 *  Copyright (C) 2005-2019 Jason Katz-Brown, John O'Laughlin, and John Fultz.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QUACKER_WORDS_H
#define QUACKER_WORDS_H

#include <QSet>
#include <QString>

#include "view.h"

class QListWidget;
class QListWidgetItem;
class QVBoxLayout;

// A simple view listing words played in the current game history.
// Clicking a word opens the configured URL in the default browser.
class Words : public HistoryView
{
Q_OBJECT

public:
    Words(QWidget *parent = 0);
    virtual ~Words() {}

public slots:
    virtual void historyChanged(const Quackle::History &history) override;

private slots:
    void itemActivated(QListWidgetItem *item);

private:
    QString buildUrlForWord(const QString &word) const;

    QVBoxLayout *m_layout;
    QListWidget *m_list;
};

#endif // QUACKER_WORDS_H
