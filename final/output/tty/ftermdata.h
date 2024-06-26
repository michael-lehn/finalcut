/***********************************************************************
* ftermdata.h - Data class for FTerm                                   *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2018-2022 Markus Gans                                      *
*                                                                      *
* FINAL CUT is free software; you can redistribute it and/or modify    *
* it under the terms of the GNU Lesser General Public License as       *
* published by the Free Software Foundation; either version 3 of       *
* the License, or (at your option) any later version.                  *
*                                                                      *
* FINAL CUT is distributed in the hope that it will be useful, but     *
* WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU Lesser General Public License for more details.                  *
*                                                                      *
* You should have received a copy of the GNU Lesser General Public     *
* License along with this program.  If not, see                        *
* <http://www.gnu.org/licenses/>.                                      *
***********************************************************************/

/*  Standalone class
 *  ════════════════
 *
 * ▕▔▔▔▔▔▔▔▔▔▔▔▏
 * ▕ FTermData ▏
 * ▕▁▁▁▁▁▁▁▁▁▁▁▏
 */

#ifndef FTERMDATA_H
#define FTERMDATA_H

#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

#include <atomic>
#include <bitset>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "final/fc.h"
#include "final/ftypes.h"
#include "final/util/frect.h"
#include "final/util/fstring.h"

namespace finalcut
{

//----------------------------------------------------------------------
// class FCharSubstitution
//----------------------------------------------------------------------

class FCharSubstitution
{
  public:
    struct Map
    {
      wchar_t from;
      wchar_t to;
    };

    auto getMappedChar (wchar_t) const -> wchar_t;
    void setCharMapping (const Map&);
    auto isEmpty() const -> bool;
    void sort();

  private:
    std::vector<Map> sub_map{};
};

// FTermData inline functions
//----------------------------------------------------------------------
inline auto FCharSubstitution::getMappedChar (wchar_t c) const -> wchar_t
{
  const auto& cend = sub_map.cend();
  auto iter = std::find_if ( sub_map.cbegin(), cend,
                             [&c] (const auto& map)
                             {
                               return map.from == c;
                             } );

  if ( iter == cend )
    return L'\0';

  return iter->to;
}

//----------------------------------------------------------------------
inline void FCharSubstitution::setCharMapping (const Map& m)
{
  const auto& end = sub_map.end();
  auto iter = std::find_if ( sub_map.begin(), end,
                             [&m] (const auto& map)
                             {
                               return map.from == m.from;
                             } );

  if ( iter == end )
    sub_map.push_back(m);
  else
    iter->to = m.to;
}

//----------------------------------------------------------------------
inline auto FCharSubstitution::isEmpty() const -> bool
{
  return sub_map.empty();
}

//----------------------------------------------------------------------
inline void FCharSubstitution::sort()
{
  std::sort ( sub_map.begin(), sub_map.end()
            , [] (const auto& lhs, const auto& rhs)
              {
                return lhs.from < rhs.from;
              }
            );
}


//----------------------------------------------------------------------
// class FTermData
//----------------------------------------------------------------------

class FTermData final
{
  public:
    struct kittyVersion
    {
      int primary;
      int secondary;
    };

    // Using-declaration
    using EncodingMap = std::unordered_map<std::string, Encoding>;

    // Constructors
    FTermData () = default;

    // Accessors
    auto getClassName() const -> FString;
    static auto getInstance() -> FTermData&;
    auto getEncodingList() & -> EncodingMap&;
    auto getCharSubstitutionMap() & -> FCharSubstitution&;
    auto getTerminalEncoding() const -> Encoding;
    auto getTerminalGeometry() & -> FRect&;
    auto getTerminalGeometry() const & -> const FRect&;
    auto getTTYFileDescriptor() const noexcept -> int;
    auto getBaudrate() const noexcept -> uInt;
    auto getTermType() const & -> const std::string& ;
    auto getTermFileName() const & -> const std::string&;
    auto getGnomeTerminalID() const noexcept -> int;
    auto getKittyVersion() const noexcept -> kittyVersion;
    auto getXtermFont() const & -> const FString&;
    auto getXtermTitle() const & -> const FString&;
#if DEBUG
    auto getFramebufferBpp() const noexcept -> int;
#endif

    // Inquiries
    auto hasShadowCharacter() const noexcept -> bool;
    auto hasHalfBlockCharacter() const noexcept -> bool;
    auto hasCursorOptimisation() const noexcept -> bool;
    auto isCursorHidden() const noexcept -> bool;
    auto hasAlternateScreen() const noexcept -> bool;
    auto isInAlternateScreen() const noexcept -> bool;
    auto hasASCIIConsole() const noexcept -> bool;
    auto hasVT100Console() const noexcept -> bool;
    auto hasUTF8Console() const noexcept -> bool;
    auto isUTF8() const noexcept -> bool;
    auto isNewFont() const noexcept -> bool;
    auto isVGAFont() const noexcept -> bool;
    auto isMonochron() const noexcept -> bool;
    auto hasTermResized() -> bool;
    auto isTermType (FTermType) const -> bool;
    auto isTermType (FTermTypeT) const -> bool;

    // Mutators
    void setTermEncoding (Encoding) noexcept;
    void setTTYFileDescriptor (int) noexcept;
    void setBaudrate (uInt) noexcept;
    void supportShadowCharacter (bool = true) noexcept;
    void supportHalfBlockCharacter (bool = true) noexcept;
    void supportCursorOptimisation (bool = true) noexcept;
    void setCursorHidden (bool = true) noexcept;
    void useAlternateScreen (bool = true) noexcept;
    void setAlternateScreenInUse (bool = true) noexcept;
    void setASCIIConsole (bool = true) noexcept;
    void setVT100Console (bool = true) noexcept;
    void setUTF8Console (bool = true) noexcept;
    void setUTF8 (bool = true) noexcept;
    void setNewFont (bool = true) noexcept;
    void setVGAFont (bool = true) noexcept;
    void setMonochron (bool = true) noexcept;
    void setTermResized (bool = true);
    void setTermType (const std::string&);
    void setTermType (FTermType);
    void unsetTermType (FTermType);
    void setTermFileName (const std::string&);
    void setGnomeTerminalID (int) noexcept;
    void setKittyVersion (const kittyVersion&);
    void setXtermFont (const FString&);
    void setXtermTitle (const FString&);
#if DEBUG
    void setFramebufferBpp (int) noexcept;
#endif

  private:
    // Data members
    EncodingMap        encoding_list{};
    FCharSubstitution  char_substitution_map{};
    FRect              terminal_geometry{};  // current terminal geometry
    FString            xterm_font{};
    FString            xterm_title{};
    FString            exit_message{};
    FTermTypeT         terminal_type{};
    Encoding           term_encoding{Encoding::Unknown};

    // Teletype (tty) file descriptor is still undefined (-1)
    int                fd_tty{-1};

    // Gnome terminal id from SecDA
    // Example: vte version 0.40.0 = 0 * 100 + 40 * 100 + 0 = 4000
    //                      a.b.c  = a * 100 +  b * 100 + c
    int                gnome_terminal_id{0};
    kittyVersion       kitty_version{0, 0};

#if DEBUG
    int                framebuffer_bpp{-1};
#endif

    uInt               baudrate{0};
    std::string        termtype{};
    std::string        termfilename{};
    std::mutex         resize_mutex{};
    std::atomic<int>   resize_count{0};
    bool               shadow_character{true};
    bool               half_block_character{true};
    bool               cursor_optimisation{true};
    bool               hidden_cursor{false};  // Global cursor hidden state
    bool               use_alternate_screen{true};
    bool               alternate_screen{false};
    bool               ascii_console{false};
    bool               vt100_console{false};
    bool               utf8_console{false};
    bool               utf8_state{false};
    bool               new_font{false};
    bool               vga_font{false};
    bool               monochron{false};
};

// FTermData inline functions
//----------------------------------------------------------------------
inline auto FTermData::getClassName() const -> FString
{ return "FTermData"; }

//----------------------------------------------------------------------
inline auto FTermData::getInstance() -> FTermData&
{
  static const auto& data = std::make_unique<FTermData>();
  return *data;
}

//----------------------------------------------------------------------
inline auto FTermData::getEncodingList() & -> EncodingMap&
{ return encoding_list; }

//----------------------------------------------------------------------
inline auto FTermData::getCharSubstitutionMap() & -> FCharSubstitution&
{ return char_substitution_map; }

//----------------------------------------------------------------------
inline auto FTermData::getTerminalEncoding() const -> Encoding
{ return term_encoding; }

//----------------------------------------------------------------------
inline auto FTermData::getTerminalGeometry() & -> FRect&
{ return terminal_geometry; }

//----------------------------------------------------------------------
inline auto FTermData::getTerminalGeometry() const & -> const FRect&
{ return terminal_geometry; }

//----------------------------------------------------------------------
inline auto FTermData::getTTYFileDescriptor() const noexcept -> int
{ return fd_tty; }

//----------------------------------------------------------------------
inline auto FTermData::getBaudrate() const noexcept -> uInt
{ return baudrate; }

//----------------------------------------------------------------------
inline auto FTermData::getTermType() const & -> const std::string&
{ return termtype; }

//----------------------------------------------------------------------
inline auto FTermData::getTermFileName() const & -> const std::string&
{ return termfilename; }

//----------------------------------------------------------------------
inline auto FTermData::getGnomeTerminalID() const noexcept -> int
{ return gnome_terminal_id; }

//----------------------------------------------------------------------
inline auto FTermData::getKittyVersion() const noexcept -> kittyVersion
{ return kitty_version; }

//----------------------------------------------------------------------
inline auto FTermData::getXtermFont() const & -> const FString&
{ return xterm_font; }

//----------------------------------------------------------------------
inline auto FTermData::getXtermTitle() const & -> const FString&
{ return xterm_title; }

//----------------------------------------------------------------------
#if DEBUG
inline auto FTermData::getFramebufferBpp() const noexcept -> int
{ return framebuffer_bpp; }
#endif

//----------------------------------------------------------------------
inline auto FTermData::hasShadowCharacter() const noexcept -> bool
{ return shadow_character; }

//----------------------------------------------------------------------
inline auto FTermData::hasHalfBlockCharacter() const noexcept -> bool
{ return half_block_character; }

//----------------------------------------------------------------------
inline auto FTermData::hasCursorOptimisation() const noexcept -> bool
{ return cursor_optimisation; }

//----------------------------------------------------------------------
inline auto FTermData::isCursorHidden() const noexcept -> bool
{ return hidden_cursor; }

//----------------------------------------------------------------------
inline auto FTermData::hasAlternateScreen() const noexcept -> bool
{ return use_alternate_screen; }

//----------------------------------------------------------------------
inline auto FTermData::isInAlternateScreen() const noexcept -> bool
{ return alternate_screen; }

//----------------------------------------------------------------------
inline auto FTermData::hasASCIIConsole() const noexcept -> bool
{ return ascii_console; }

//----------------------------------------------------------------------
inline auto FTermData::hasVT100Console() const noexcept -> bool
{ return vt100_console; }

//----------------------------------------------------------------------
inline auto FTermData::hasUTF8Console() const noexcept -> bool
{ return utf8_console; }

//----------------------------------------------------------------------
inline auto FTermData::isUTF8() const noexcept -> bool
{ return utf8_state; }

//----------------------------------------------------------------------
inline auto FTermData::isNewFont() const noexcept -> bool
{ return new_font; }

//----------------------------------------------------------------------
inline auto FTermData::isVGAFont() const noexcept -> bool
{ return vga_font; }

//----------------------------------------------------------------------
inline auto FTermData::isMonochron() const noexcept -> bool
{ return monochron; }

//----------------------------------------------------------------------
inline auto FTermData::hasTermResized() -> bool
{
  std::lock_guard<std::mutex> resize_lock_guard(resize_mutex);
  return resize_count.load() > 0;
}

//----------------------------------------------------------------------
inline auto FTermData::isTermType (FTermType type) const -> bool
{ return terminal_type & static_cast<FTermTypeT>(type); }

//----------------------------------------------------------------------
inline auto FTermData::isTermType (FTermTypeT mask) const -> bool
{ return terminal_type & mask; }

//----------------------------------------------------------------------
inline void FTermData::setTermEncoding (Encoding enc) noexcept
{ term_encoding = enc; }

//----------------------------------------------------------------------
inline void FTermData::setTTYFileDescriptor (int fd) noexcept
{ fd_tty = fd; }

//----------------------------------------------------------------------
inline void FTermData::setBaudrate (uInt baud) noexcept
{ baudrate = baud; }

//----------------------------------------------------------------------
inline void FTermData::supportShadowCharacter (bool available) noexcept
{ shadow_character = available; }

//----------------------------------------------------------------------
inline void FTermData::supportHalfBlockCharacter (bool available) noexcept
{ half_block_character = available; }

//----------------------------------------------------------------------
inline void FTermData::supportCursorOptimisation (bool available) noexcept
{ cursor_optimisation = available; }

//----------------------------------------------------------------------
inline void FTermData::setCursorHidden (bool hidden_state) noexcept
{ hidden_cursor = hidden_state; }

//----------------------------------------------------------------------
inline void FTermData::useAlternateScreen (bool use) noexcept
{ use_alternate_screen = use; }

//----------------------------------------------------------------------
inline void FTermData::setAlternateScreenInUse (bool in_use) noexcept
{ alternate_screen = in_use; }

//----------------------------------------------------------------------
inline void FTermData::setASCIIConsole (bool ascii) noexcept
{ ascii_console = ascii; }

//----------------------------------------------------------------------
inline void FTermData::setVT100Console (bool vt100) noexcept
{ vt100_console = vt100; }

//----------------------------------------------------------------------
inline void FTermData::setUTF8Console (bool utf8) noexcept
{ utf8_console = utf8; }

//----------------------------------------------------------------------
inline void FTermData::setUTF8 (bool utf8) noexcept
{ utf8_state = utf8; }

//----------------------------------------------------------------------
inline void FTermData::setNewFont (bool nfont) noexcept
{ new_font = nfont; }

//----------------------------------------------------------------------
inline void FTermData::setVGAFont (bool vga) noexcept
{ vga_font = vga; }

//----------------------------------------------------------------------
inline void FTermData::setMonochron (bool mono) noexcept
{ monochron = mono; }

//----------------------------------------------------------------------
inline void FTermData::setTermResized (bool resize)
{
  std::lock_guard<std::mutex> resize_lock_guard(resize_mutex);

  if ( resize )
    ++resize_count;
  else if ( resize_count.load() > 0 )
    --resize_count;
}

//----------------------------------------------------------------------
inline void FTermData::setTermType (const std::string& name)
{
  if ( ! name.empty() )
    termtype = name;
}

//----------------------------------------------------------------------
inline void FTermData::setTermType (FTermType type)
{ terminal_type |= static_cast<FTermTypeT>(type); }

//----------------------------------------------------------------------
inline void FTermData::unsetTermType (FTermType type)
{ terminal_type &= ~(static_cast<FTermTypeT>(type)); }

//----------------------------------------------------------------------
inline void FTermData::setTermFileName (const std::string& file_name)
{
  if ( ! file_name.empty() )
    termfilename = file_name;
}

//----------------------------------------------------------------------
inline void FTermData::setGnomeTerminalID (int id) noexcept
{ gnome_terminal_id = id; }

//----------------------------------------------------------------------
inline void FTermData::setKittyVersion(const kittyVersion& version)
{
  kitty_version.primary = version.primary;
  kitty_version.secondary = version.secondary;
}

//----------------------------------------------------------------------
inline void FTermData::setXtermFont (const FString& font)
{ xterm_font = font; }

//----------------------------------------------------------------------
inline void FTermData::setXtermTitle (const FString& title)
{ xterm_title = title; }

//----------------------------------------------------------------------
#if DEBUG && defined(__linux__)
inline void FTermData::setFramebufferBpp (int bpp) noexcept
{ framebuffer_bpp = bpp; }
#endif

}  // namespace finalcut

#endif  // FTERMDATA_H
