/**
 * @brief PFS library - Frame
 *
 * This file is a part of Luminance HDR package.
 * ----------------------------------------------------------------------
 * Copyright (C) 2003,2004 Rafal Mantiuk and Grzegorz Krawczyk
 * Copyright (C) 2011 Davide Anastasia
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * ----------------------------------------------------------------------
 *
 * @author Rafal Mantiuk, <mantiuk@mpi-sb.mpg.de>
 * @author Davide Anastasia <davideanastasia@users.sourceforge.net>
 *   Frame definition split from pfs.cpp
 */

#ifndef __PFS_FRAME_H__
#define __PFS_FRAME_H__

#include <string>

#include "channel.h"
#include "tag.h"

namespace pfs
{
    /**
 * Interface representing a single PFS frame. Frame may contain 0
 * or more channels (e.g. color XYZ, depth channel, alpha
 * channnel). All the channels are of the same size. Frame can
 * also contain additional information in tags (see getTags).
 */
    class Frame
    {
    public:
        /**
     * Gets width of the channels (in pixels).
     */
        virtual int getWidth() const = 0;

        /**
     * Gets height of the channels (in pixels).
     */
        virtual int getHeight() const = 0;

        /**
     * Gets color channels in XYZ color space. May return NULLs
     * if such channels do not exist. Values assigned to
     * X, Y, Z are always either all NULLs or valid pointers to
     * channels.
     *
     * @param X [out] a pointer to store X channel in
     * @param Y [out] a pointer to store Y channel in
     * @param Z [out] a pointer to store Z channel in
     */
        virtual void getXYZChannels( Channel* &X, Channel* &Y, Channel* &Z ) = 0;

        /**
     * Creates color channels in XYZ color space. If such channels
     * already exists, returns existing channels, rather than
     * creating new ones.  Note, that nothing can be assumed about
     * the content of each channel.
     *
     * @param X [out] a pointer to store X channel in
     * @param Y [out] a pointer to store Y channel in
     * @param Z [out] a pointer to store Z channel in
     */
        virtual void createXYZChannels( Channel* &X, Channel* &Y, Channel* &Z ) = 0;

        /**
     * Gets a named channel.
     *
     * @param name [in] name of the channel. Name must be 8 or less
     * character long.
     * @return channel or NULL if the channel does not exist
     */
        virtual Channel* getChannel( std::string name ) = 0; //const char *name ) = 0;

        /**
     * Creates a named channel. If the channel already exists, returns
     * existing channel.
     *
     * Note that new channels should be created only for the first
     * frame. The channels should not changes for the subsequent
     * frames of a sequence.
     *
     * @param name [in] name of the channel. Name must be 8 or less
     * character long.
     * @return existing or newly created channel
     */
        virtual Channel* createChannel( std::string name ) = 0; //const char *name ) = 0;


        /**
     * Removes a channel. It is safe to remove the channel pointed by
     * the ChannelIterator.
     *
     * @param channel [in] channel that should be removed.
     */
        virtual void removeChannel( Channel *channel ) = 0;

        /**
     * DEPRECATED!!! Use getIterator instead.
     *
     * Returns iterator for all available channels.
     *
     * Note that only one iterator for particular frame can be used at
     * a time. This method returns each time the same data structure,
     * so the iterator from previous call is lost after the call. The
     * iterator MUST NOT be deleted after use.
     *
     * Object ChannelIterator MUST NOT be freed. It's responsibility
     * of a Frame object.
     */
        virtual ChannelIterator *getChannels() = 0;

        /**
     * Use ChannelIterator to iterate over all Channels in the Frame.
     * ChannelIteratorPtr is a smart pointer, which destructs
     * ChannelIterator when ChannelIteratorPtr is destructed. Use ->
     * operator to access ChannelIterator members from a
     * ChannelIteratorPtr object.
     *
     * To iterate over all channels, use the following code:
     * <code>
     * pfs::ChannelIteratorPtr it( frame->getChannelIterator() );
     * while( it->hasNext() ) {
     *   pfs::Channel *ch = cit->getNext();
     *   //Do whatever is needed
     * }
     * </code>
     */
        virtual ChannelIteratorPtr getChannelIterator() = 0;

        /**
     * Returns TagContainer that can be used to access or modify
     * tags associated with this Frame object.
     */
        virtual TagContainer *getTags() = 0;

        // A pure virtual destructor
        virtual ~Frame() { }

    };


    //------------------------------------------------------------------------------
    // Frame implementation
    //------------------------------------------------------------------------------
    class FrameImpl: public Frame
    {
        int width, height;

    protected:
        friend class DOMIO;

        TagContainerImpl *tags;

        ChannelMap channel;
        ChannelIteratorImpl channelIterator;

    public:

        FrameImpl( int width, int height );
        virtual ~FrameImpl();

        virtual int getWidth() const;
        virtual int getHeight() const;

        virtual void getXYZChannels( Channel* &X, Channel* &Y, Channel* &Z );
        virtual void createXYZChannels( Channel* &X, Channel* &Y, Channel* &Z );

        Channel* getChannel( std::string name ); //const char *name )
        Channel *createChannel( std::string name ); //const char *name )
        void removeChannel( Channel *ch );

        ChannelIterator *getChannels();
        ChannelIteratorPtr getChannelIterator();

        TagContainer *getTags();
    };
}

#endif // __FRAME_H__