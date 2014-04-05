/****************************************************************************
**
** Copyright (C) 2012 Giuseppe D'Angelo <dangelog@gmail.com>.
** Contact: http://www.qt-project.org/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QREGULAREXPRESSION_H
#define QREGULAREXPRESSION_H

#include <QtCore/QString>
#include <QtCore/QSharedData>
#include <QtCore/QVariant>

class QRegularExpressionMatch;
class QRegularExpressionMatchIterator;
struct QRegularExpressionPrivate;

class QRegularExpression
{
public:
    enum PatternOption {
        NoPatternOption                = 0x0000,
        CaseInsensitiveOption          = 0x0001,
        DotMatchesEverythingOption     = 0x0002,
        MultilineOption                = 0x0004,
        ExtendedPatternSyntaxOption    = 0x0008,
        InvertedGreedinessOption       = 0x0010,
        DontCaptureOption              = 0x0020,
        UseUnicodePropertiesOption     = 0x0040
    };
    Q_DECLARE_FLAGS(PatternOptions, PatternOption)

    PatternOptions patternOptions() const;
    void setPatternOptions(PatternOptions options);

    QRegularExpression();
    explicit QRegularExpression(const QString &pattern, PatternOptions options = NoPatternOption);
    QRegularExpression(const QRegularExpression &re);
    ~QRegularExpression();
    QRegularExpression &operator=(const QRegularExpression &re);

#ifdef Q_COMPILER_RVALUE_REFS
    inline QRegularExpression &operator=(QRegularExpression &&re)
    { d.swap(re.d); return *this; }
#endif

    inline void swap(QRegularExpression &re) { d.swap(re.d); }

    QString pattern() const;
    void setPattern(const QString &pattern);

    bool isValid() const;
    int patternErrorOffset() const;
    QString errorString() const;

    int captureCount() const;

    enum MatchType {
        NormalMatch = 0,
        PartialPreferCompleteMatch,
        PartialPreferFirstMatch
    };

    enum MatchOption {
        NoMatchOption              = 0x0000,
        AnchoredMatchOption        = 0x0001
    };
    Q_DECLARE_FLAGS(MatchOptions, MatchOption)

    QRegularExpressionMatch match(const QString &subject,
                                  int offset                = 0,
                                  MatchType matchType       = NormalMatch,
                                  MatchOptions matchOptions = NoMatchOption) const;

    QRegularExpressionMatchIterator globalMatch(const QString &subject,
                                                int offset                = 0,
                                                MatchType matchType       = NormalMatch,
                                                MatchOptions matchOptions = NoMatchOption) const;

    static QString escape(const QString &str);

    bool operator==(const QRegularExpression &re) const;
    inline bool operator!=(const QRegularExpression &re) const { return !operator==(re); }

private:
    friend struct QRegularExpressionPrivate;
    friend class QRegularExpressionMatch;
    friend struct QRegularExpressionMatchPrivate;
    friend class QRegularExpressionMatchIterator;

    QRegularExpression(QRegularExpressionPrivate &dd);
    QExplicitlySharedDataPointer<QRegularExpressionPrivate> d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QRegularExpression::PatternOptions)
Q_DECLARE_OPERATORS_FOR_FLAGS(QRegularExpression::MatchOptions)
Q_DECLARE_TYPEINFO(QRegularExpression, Q_MOVABLE_TYPE);

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const QRegularExpression &re);
QDataStream &operator>>(QDataStream &in, QRegularExpression &re);
#endif

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const QRegularExpression &re);
QDebug operator<<(QDebug debug, QRegularExpression::PatternOptions patternOptions);
#endif

struct QRegularExpressionMatchPrivate;

class QRegularExpressionMatch
{
public:
    ~QRegularExpressionMatch();
    QRegularExpressionMatch(const QRegularExpressionMatch &match);
    QRegularExpressionMatch &operator=(const QRegularExpressionMatch &match);

#ifdef Q_COMPILER_RVALUE_REFS
    inline QRegularExpressionMatch &operator=(QRegularExpressionMatch &&match)
    { d.swap(match.d); return *this; }
#endif
    inline void swap(QRegularExpressionMatch &match) { d.swap(match.d); }

    QRegularExpression regularExpression() const;
    QRegularExpression::MatchType matchType() const;
    QRegularExpression::MatchOptions matchOptions() const;

    bool hasMatch() const;
    bool hasPartialMatch() const;

    bool isValid() const;

    int lastCapturedIndex() const;

    QString captured(int nth = 0) const;
    QStringRef capturedRef(int nth = 0) const;

    QString captured(const QString &name) const;
    QStringRef capturedRef(const QString &name) const;

    QStringList capturedTexts() const;

    int capturedStart(int nth = 0) const;
    int capturedLength(int nth = 0) const;
    int capturedEnd(int nth = 0) const;

    int capturedStart(const QString &name) const;
    int capturedLength(const QString &name) const;
    int capturedEnd(const QString &name) const;

private:
    friend class QRegularExpression;
    friend struct QRegularExpressionMatchPrivate;
    friend class QRegularExpressionMatchIterator;

    QRegularExpressionMatch(QRegularExpressionMatchPrivate &dd);
    QSharedDataPointer<QRegularExpressionMatchPrivate> d;
};

Q_DECLARE_TYPEINFO(QRegularExpressionMatch, Q_MOVABLE_TYPE);

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const QRegularExpressionMatch &match);
#endif

struct QRegularExpressionMatchIteratorPrivate;

class QRegularExpressionMatchIterator
{
public:
    ~QRegularExpressionMatchIterator();
    QRegularExpressionMatchIterator(const QRegularExpressionMatchIterator &iterator);
    QRegularExpressionMatchIterator &operator=(const QRegularExpressionMatchIterator &iterator);
#ifdef Q_COMPILER_RVALUE_REFS
    inline QRegularExpressionMatchIterator &operator=(QRegularExpressionMatchIterator &&iterator)
    { d.swap(iterator.d); return *this; }
#endif
    void swap(QRegularExpressionMatchIterator &iterator) { d.swap(iterator.d); }

    bool isValid() const;

    bool hasNext() const;
    QRegularExpressionMatch next();
    QRegularExpressionMatch peekNext() const;

    QRegularExpression regularExpression() const;
    QRegularExpression::MatchType matchType() const;
    QRegularExpression::MatchOptions matchOptions() const;

private:
    friend class QRegularExpression;

    QRegularExpressionMatchIterator(QRegularExpressionMatchIteratorPrivate &dd);
    QSharedDataPointer<QRegularExpressionMatchIteratorPrivate> d;
};

Q_DECLARE_TYPEINFO(QRegularExpressionMatchIterator, Q_MOVABLE_TYPE);

#endif // QREGULAREXPRESSION_H
