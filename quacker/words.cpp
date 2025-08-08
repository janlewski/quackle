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

#include <QtWidgets>
#include <QDesktopServices>
#include <QUrl>

#include <game.h>
#include <quackleio/util.h>

#include "words.h"
#include "geometry.h"
#include "customqsettings.h"
#include "quackersettings.h"

Words::Words(QWidget *parent)
    : HistoryView(parent)
{
    m_layout = new QVBoxLayout(this);
    Geometry::setupFramedLayout(m_layout);

    m_list = new QListWidget(this);
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setUniformItemSizes(true);
    connect(m_list, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(itemActivated(QListWidgetItem *)));

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    m_layout->addWidget(m_list);
}

void Words::historyChanged(const Quackle::History &history)
{
    m_list->clear();

    // Iterate over all positions and collect words formed by committed moves
    // Preserve the chronological order and include duplicates.
    for (const auto &pos : history)
    {
        const Quackle::Move &cm = pos.committedMove();
        if (cm.action == Quackle::Move::Place && !cm.isChallengedPhoney())
        {
            // Collect all words formed (main and cross) for this move
            Quackle::MoveList formed = pos.allWordsFormedBy(cm);
            for (const auto &wmove : formed)
            {
                QString w = QuackleIO::Util::letterStringToQString(wmove.wordTiles());
                if (!w.isEmpty())
                    m_list->addItem(w);
            }
        }
    }
}

QString Words::buildUrlForWord(const QString &word) const
{
    // Read template from settings, default "sjp.pl/$1"
    CustomQSettings settings;
    const QString templ = settings.value("quackle/settings/word-lookup-template", QString("sjp.pl/$1")).toString();

    // Percent-encode the word (lowercase) and substitute into the template.
    const QByteArray enc = QUrl::toPercentEncoding(word.toLower());
    QString url = templ;
    url.replace("$1", QString::fromUtf8(enc));

    // Prepend scheme if missing
    if (!url.startsWith("http://") && !url.startsWith("https://"))
        url = QString("https://") + url;
    return url;
}

void Words::itemActivated(QListWidgetItem *item)
{
    if (!item)
        return;
    const QString word = item->text();
    const QString url = buildUrlForWord(word);
    QDesktopServices::openUrl(QUrl(url));
}
