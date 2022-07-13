/*******************************************************************************

   Tests for the ChunkContainer, used by the tape.

   This file is part of XAD, a fast and comprehensive C++ library for
   automatic differentiation.

   Copyright (C) 2010-2022 Xcelerit Computing Ltd.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#include <XAD/ChunkContainer.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace ::testing;
using xad::ChunkContainer;

TEST(ChunkContainer, iterator)
{
    ChunkContainer<int> chk;
    for (int i = 0; i < 10; ++i) chk.push_back(i);

    auto it = chk.iterator_at(5);
    auto itend = chk.iterator_at(10);
    int i = 5;
    while (it != itend)
    {
        EXPECT_EQ(i, *it);
        ++it;
        ++i;
    }
}

TEST(ChunkContainer, iterator_over_end)
{
    ChunkContainer<int> chk;
    for (int i = 0; i < int(ChunkContainer<int>::chunk_size + 5); ++i) chk.push_back(i);

    auto it = chk.iterator_at(ChunkContainer<int>::chunk_size - 4);
    auto itend = chk.iterator_at(ChunkContainer<int>::chunk_size + 5);
    int i = int(ChunkContainer<int>::chunk_size - 4);
    while (it != itend)
    {
        EXPECT_EQ(i, *it);
        ++it;
        ++i;
    }
}

TEST(ChunkContainer, uninitialized_extend)
{
    ChunkContainer<int> chk;
    std::size_t i = 0;
    for (; i < ChunkContainer<int>::chunk_size - 4; ++i) chk.push_back(int(i));
    chk.uninitialized_extend(10);
    EXPECT_EQ(ChunkContainer<int>::chunk_size - 4 + 10, chk.size());
    auto it = chk.iterator_at(i);
    for (std::size_t j = i + 10; i < j; ++i)
    {
        ::new (&*it++) int(static_cast<int>(i));
    }

    for (std::size_t j = 0; j < ChunkContainer<int>::chunk_size - 4 + 10; ++j)
        EXPECT_EQ(int(j), chk[j]);
}

TEST(ChunkContainer, move_construct)
{
    ChunkContainer<int> chk;
    chk.push_back(123);

    auto addr = &chk[0];

    ChunkContainer<int> chk2(std::move(chk));

    EXPECT_THAT(chk2[0], Eq(123));
    EXPECT_THAT(addr, Eq(&chk2[0]));
}

TEST(ChunkContainer, move_assign)
{
    ChunkContainer<int> chk;
    chk.push_back(123);

    auto addr = &chk[0];

    ChunkContainer<int> chk2;
    chk2.push_back(42);
    auto addr2 = &chk2[0];
    chk2 = std::move(chk);

    EXPECT_THAT(chk2[0], Eq(123));
    EXPECT_THAT(addr, Eq(&chk2[0]));
    EXPECT_THAT(addr, Ne(addr2));
}