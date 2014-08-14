///
///  FFGrid.hpp
///
///  Created by Baptiste Dupy on 13/08/2014.
///  Contact:
///          Mail:       <baptiste.dupy@gmail.com>
///          GitHub:     trfd <https://github.com/trfd>
///          Website:    <http://www.trf-d.com>
///
///  Copyright (C) 2014 Baptiste (trfd) Dupy
///
///  This program is free software: you can redistribute it and/or modify
///  it under the terms of the GNU General Public License as published by
///  the Free Software Foundation, either version 3 of the License, or
///  (at your option) any later version.
///
///  This program is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///  GNU General Public License for more details.
///
///  You should have received a copy of the GNU General Public License
///  along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
///
///

#ifndef AI_FF_Grid_hpp
#define AI_FF_Grid_hpp

#include <memory>
#include <exception>
#include <string>

#include "FFChunk.hpp"

#include "ai/CKUMReturnType.hpp"

using namespace ck::utils::meta;

namespace ai
{
    namespace ff
    {
        class NullChunkException : public std::exception 
        {
        private:
            int m_cx; int m_cy;
        public:
            NullChunkException(int cx,int cy) 
            :m_cx(cx),m_cy(cy){}
            const char* what() {
                return ("Try to access null chunk (" + 
                        std::to_string(m_cx) + ","   +
                        std::to_string(m_cy) + ")").c_str();
            }
        };

        /// Classic 2D grid based on fixed number of chunks
        /// This grid uses shared pointer to chunks in 
        /// order to allow dynamic creation/swap/loading
        /// of chunks.
        template<typename _ChunkType,int __width, int __height>
        class Grid
        {
        public:

            typedef _ChunkType                      Chunk;
            typedef typename Chunk::Data            Data;
            typedef std::shared_ptr<_ChunkType>     Chunk_ptr;
            typedef typename ReturnType<Data>::type ReturnType;

            static const int width  = __width;
            static const int height = __height;
            static const int size   = __width*__height;

            static const int chunkWidth  = Chunk::width;
            static const int chunkHeight = Chunk::height;
            static const int chunkSize   = Chunk::size;

            static const int chunkRowCount      = width / Chunk::width;
            static const int chunkColumnCount   = height / Chunk::height;
            static const int chunkCount         = chunkRowCount*chunkColumnCount;

            virtual ~Grid(){}

            #pragma region Cell Interface

            inline bool isInGrid(int x_, int y_)
            {
                return (x_>=0 && y_>=0 && x_ < __width && y_ < __height);
            }

            inline void set(Data&& d_, int x_, int y_)
            {
                if(!isInGrid(x_,y_))
                    return;

                Chunk_ptr& chunk = chunkAt(x_, y_);

                if(!chunk.get())
                    throw NullChunkException(x_ / chunkWidth, y_ / chunkHeight);
                
                chunk->set(std::forward<Data>(d_), x_%chunkWidth, y_%chunkHeight);
            }

            inline void set(Data const& d_, int x_, int y_)
            {
                if(!isInGrid(x_, y_))
                    return;

                Chunk_ptr& chunk = chunkAt(x_, y_);

                if(!chunk.get())
                    throw NullChunkException(x_ / chunkWidth, y_ / chunkHeight);

                chunk->set(d_, x_%chunkWidth, y_%chunkHeight);
            }

            inline ReturnType get(int x_, int y_)
            {
                if(!isInGrid(x_, y_))
                    return ReturnType();

                Chunk_ptr& chunk = chunkAt(x_, y_);

                if(!chunk)
                    throw NullChunkException(x_ / chunkWidth, y_ / chunkHeight);

                return chunk->get(x_%chunkWidth, y_%chunkHeight);
            }

            #pragma endregion

            #pragma region Chunk Interface

            inline int indexOfChunkAt(int x_, int y_)
            {
                return  x_ / chunkWidth + chunkRowCount * (y_ / chunkHeight);
            }

            // No safety checks
            inline Chunk_ptr& chunkAt(int x_, int y_)
            {
                return m_chunks[indexOfChunkAt(x_,y_)];
            }

            // No safety checks
            inline Chunk_ptr& chunk(int idx_)
            {
                return m_chunks[idx_];
            }

            inline void setChunk(Chunk_ptr const& chunk_, int idx_)
            {
                m_chunks[idx_] = chunk_;
            }
            #pragma endregion

        private:
            Chunk_ptr m_chunks[chunkCount];
        };
    }
}

#endif //AI_FF_Grid_hpp