// Copyright James Puleo 2020
// Copyright Riverbend 2020

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

// This file contains all the build information.
// It should probably be automatically generated, but I am
// writing it by hand for now, as this isn't nearly stable
// enough for rolling release just yet.

#define BUILD_GIT_HASH "Debug"
#define BUILD_GIT_HASH_SHORT "Debug"
// Remembering a codename is easier than a commit hash, no? :^)
#define BUILD_CODENAME "Debug"

static const char* get_build_title()
{
    return "Riverbend-" BUILD_CODENAME "-" BUILD_GIT_HASH_SHORT;
}
