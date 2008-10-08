/*
 * akunambolview.h
 *
 * Copyright (C) 2007 Riccardo Iaconelli <riccardo@kde.org>
 */
#ifndef AKUNAMBOLVIEW_H
#define AKUNAMBOLVIEW_H

#include <QtGui/QWidget>

#include "ui_akunambolview_base.h"

class QPainter;
class KUrl;

/**
 * This is the main view class for akunambol.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author Riccardo Iaconelli <riccardo@kde.org>
 * @version 0.1
 */

class akunambolView : public QWidget, public Ui::akunambolview_base
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    akunambolView(QWidget *parent);

    /**
     * Destructor
     */
    virtual ~akunambolView();

private:
    Ui::akunambolview_base ui_akunambolview_base;

signals:
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString& text);

    /**
     * Use this signal to change the content of the caption
     */
    void signalChangeCaption(const QString& text);

private slots:
    void switchColors();
    void settingsChanged();
};

#endif // akunambolVIEW_H
