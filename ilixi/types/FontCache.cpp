/*
 Copyright 2010-2012 Tarik Sekmen.

 All Rights Reserved.

 Written by Tarik Sekmen <tarik@ilixi.org>,
 Denis Oliver Kropp <dok@directfb.org>.

 This file is part of ilixi.

 ilixi is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 ilixi is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with ilixi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <types/FontCache.h>
#include <lib/Util.h>
#include <core/AppBase.h>
#include <core/Logger.h>
#include <ilixiConfig.h>
#include <sstream>

namespace ilixi
{

D_DEBUG_DOMAIN( ILX_FONTCACHE, "ilixi/types/FontCache", "FontCache");

FontCache* FontCache::__instance = NULL;

FontCache*
FontCache::Instance()
{
    if (!__instance)
        __instance = new FontCache;
    return __instance;
}

FontCache::FontCache()
{
    pthread_mutex_init(&_lock, NULL);
}

FontCache::FontCache(FontCache const&)
{
}

FontCache&
FontCache::operator=(FontCache const&)
{
}

FontCache::~FontCache()
{
    pthread_mutex_destroy(&_lock);
}

unsigned int
FontCache::getKey(const char* name, int size, DFBFontAttributes attr)
{
    std::stringstream ss;
    ss << name << size << attr;
    return Hash(ss.str());
}

IDirectFBFont*
FontCache::getEntry(const char* name, int size, DFBFontAttributes attr)
{
    int len = strlen(ILIXI_FONTDIR) + 1 + strlen(name) + 6 + 1;
    char filename[len];
    IDirectFBFont *font;

    D_ASSERT(name != NULL);

    snprintf(filename, len, ILIXI_FONTDIR"%s.otf", name);
    font = getEntryFromFile(filename, size, attr);

    if (font == NULL)
    {
        snprintf(filename, len, ILIXI_FONTDIR"%s.ttf", name);
        font = getEntryFromFile(filename, size, attr);
    }

    return font;
}

void
FontCache::releaseEntry(unsigned int key)
{
    pthread_mutex_lock(&_lock);
    CacheMap::iterator it = _cache.find(key);
    if (it != _cache.end())
    {
        if (--(it->second->refs))
        {
            ILOG_TRACE(ILX_FONTCACHE);
            ILOG_DEBUG( ILX_FONTCACHE, " -> Release entry (%u)\n", key);
            pthread_mutex_unlock(&_lock);
            return;
        }
        // remove entry...
        ILOG_TRACE(ILX_FONTCACHE);
        ILOG_DEBUG(ILX_FONTCACHE, " -> Destroy entry (%u)\n", key);
        it->second->Release(it->second);
        _cache.erase(it);
    }
    pthread_mutex_unlock(&_lock);
}

void
FontCache::releaseEntry(const char* name, int size, DFBFontAttributes attr)
{
    releaseEntry(getKey(name, size, attr));
}

IDirectFBFont*
FontCache::getEntryFromFile(const char* name, int size, DFBFontAttributes attr)
{
    unsigned int key = getKey(name, size, attr);

    pthread_mutex_lock(&_lock);
    CacheMap::iterator it = _cache.find(key);
    if (it == _cache.end())
    {
        // load and insert
        IDirectFBFont* font;
        DFBFontDescription desc;

        desc.flags = (DFBFontDescriptionFlags) (DFDESC_HEIGHT
                | DFDESC_ATTRIBUTES);
        desc.height = size;
        desc.attributes = attr;

        DFBResult ret = AppBase::getDFB()->CreateFont(AppBase::getDFB(), name,
                                                      &desc, &font);
        if (ret)
        {
            ILOG_TRACE(ILX_FONTCACHE);
            ILOG_DEBUG(ILX_FONTCACHE,
                       " -> Loading failed for (%s, %d)!\n", name, size);
            pthread_mutex_unlock(&_lock);
            return NULL;
        }

        CacheMap::iterator it2 = _cache.begin();
        _cache.insert(it2, std::pair<unsigned int, IDirectFBFont*>(key, font));
        ILOG_TRACE(ILX_FONTCACHE);
        ILOG_DEBUG(ILX_FONTCACHE,
                   " -> Cached key (%u) for (%s, %d)\n", key, name, size);
        pthread_mutex_unlock(&_lock);

        return font;

    } else
    {
        ILOG_DEBUG(ILX_FONTCACHE,
                   "Got from cache using key (%u) (%s,%d)\n", key, name, size);
        pthread_mutex_unlock(&_lock);
        it->second->AddRef(it->second);
        return it->second;
    }
}

} /* namespace ilixi */
