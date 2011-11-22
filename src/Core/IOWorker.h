/**
 * This file is a part of Luminance HDR package.
 * ----------------------------------------------------------------------
 * Copyright (C) 2011 Davide Anastasia
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * ----------------------------------------------------------------------
 *
 * This class provides I/O of HDR images
 * This class is inspired by LoadHdrThread and borrow most of its code
 * but it is not derived from QThread
 *
 * Original Work
 * @author Davide Anastasia <davideanastasia@users.sourceforge.net>
 *
 */

#ifndef __IO_WORKER_H__
#define __IO_WORKER_H__

#ifdef __APPLE__
#include <libraw.h>
#else
#include <libraw/libraw.h>
#endif

#include <QObject>
#include <QString>
#include <QStringList>

// Forward declaration
namespace pfs {
    class Frame;
}

class HdrViewer;
class LdrViewer;

int progress_cb(void *data, enum LibRaw_progress p, int iteration, int expected);

class IOWorker : public QObject
{
    Q_OBJECT

private:
    friend int progress_cb(void *data, enum LibRaw_progress p, int iteration, int expected);

    void get_frame(QString fname);
    void emitNextStep(int iteration);
    void emitMaximumValue(int iteration);

public:
    IOWorker(QObject* parent = 0);
    ~IOWorker();

public Q_SLOTS:
    pfs::Frame* read_hdr_frame(QString filename);

    bool write_hdr_frame(pfs::Frame *frame, QString filename);
    bool write_hdr_frame(HdrViewer* frame, QString filename);

    void write_ldr_frame(LdrViewer* frame, QString filename, int quality);

signals:
    void read_hdr_failed(QString error_message);
    void read_hdr_success(pfs::Frame*, QString fname);

    void write_hdr_failed();
    void write_hdr_success(pfs::Frame*, QString);
    void write_hdr_success(HdrViewer*, QString);
    void write_ldr_failed();
    void write_ldr_success(LdrViewer*, QString);

    void setMaximum(int);
    void setValue(int);

    void IO_init();
    void IO_finish();
};

#endif
