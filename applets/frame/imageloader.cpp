/***************************************************************************
 *   Copyright  2010 by Davide Bettio <davide.bettio@kdemail.net>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "imageloader.h"

#include <QImage>
#include <kdebug.h>

#ifdef HAVE_EXIV2
#  include <exiv2/exiv2.hpp>
#endif

ImageLoader::ImageLoader(const QString &path) :
    m_path(path)
{}

QImage ImageLoader::correctRotation(const QImage& tempImage, [[maybe_unused]] const QString &path)
{
    QImage image = QImage();
    if (!tempImage.isNull()) {
#ifdef HAVE_EXIV2

#if EXIV2_TEST_VERSION(0, 28, 0)
        Exiv2::Image::UniquePtr img;
#else
        Exiv2::Image::AutoPtr img;
#endif
        std::string fileString = path.toStdString();
        img = Exiv2::ImageFactory::open(fileString);
        const Exiv2::ExifData& data = img->exifData();
        Exiv2::ExifData::const_iterator it = data.findKey(Exiv2::ExifKey("Exif.Image.Orientation"));
        int64_t orientation = 0;
        if (it != data.end()) {
            orientation = it->toInt64();
        }
        QMatrix m;
        switch (orientation) {
        case 2: // HFLIP
            m.scale(-1.0,1.0);
            image = tempImage.transformed(m);
            break;
        case 3: // ROT_180
            m.rotate(180);
            image = tempImage.transformed(m);
            break;
        case 4: // VFLIP
            m.scale(1.0,-1.0);
            image = tempImage.transformed(m);
            break;
        case 5: // ROT_90_HFLIP
            m.rotate(90);
            m.scale(-1.0,1.0);
            image = tempImage.transformed(m);
            break;
        case 6: // ROT_90
            m.rotate(90);
            image = tempImage.transformed(m);
            break;
        case 7: // ROT_90_VFLIP
            m.rotate(90);
            m.scale(1.0,-1.0);
            image = tempImage.transformed(m);
            break;
        case 8: // ROT_270
            m.rotate(270);
            image = tempImage.transformed(m);
            break;
        default:
            image = tempImage;
        }
#else
        image = tempImage;
#endif
    }
    return image;
}

void ImageLoader::run()
{
    QImage img = correctRotation(QImage(m_path), m_path);
    emit loaded(img);
}

#include "imageloader.moc"
