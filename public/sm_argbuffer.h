/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod
 * Copyright (C) 2004-2019 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */
 
/**
 * A type-safe abstraction for creating a contiguous blob of memory.
 */
template <typename T, typename...Rest>
class ArgBuffer {
public:
	ArgBuffer(T t, Rest... rest) {
		unsigned char *ptr = buff;
		buildbuffer(&ptr, t, rest...);
	}

	operator void*() { return buff; }
private:
	template <typename K>
	constexpr static int sizetypes() {
		return sizeof(K);
	}
	template <typename K, typename K2, typename... Kn>
	constexpr static int sizetypes() {
		return sizeof(K) + sizetypes<K2, Kn...>();
	}
	
	template <typename K>
	constexpr void buildbuffer(unsigned char **ptr, K k) {
		memcpy(*ptr, &k, sizeof(K));
		*ptr += sizeof(K);
	}
	
	template <typename K, typename... Kn>
	constexpr void buildbuffer(unsigned char **ptr, K k, Kn... kn) {
		buildbuffer(ptr, k);
		if (sizeof...(kn)!=0)
			buildbuffer(ptr, kn...);
	}
	
private:
	unsigned char buff[sizetypes<T, Rest...>()];
};
