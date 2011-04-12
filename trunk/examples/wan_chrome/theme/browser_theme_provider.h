
#ifndef __wan_chrome_theme_browser_theme_provider_h__
#define __wan_chrome_theme_browser_theme_provider_h__

#pragma once

#include <map>
#include <set>
#include <string>

#include "base/ref_counted.h"
#include "base/threading/non_thread_safe.h"

#include "view_framework/gfx/theme_provider.h"

class FilePath;

namespace gfx
{
    struct HSL;
}

class ResourceBundle;

class BrowserThemePack;
class Extension;

class BrowserThemeProvider : public NonThreadSafe, public ThemeProvider
{
public:
    // Public constants used in BrowserThemeProvider and its subclasses:

    // Strings used in alignment properties.
    static const char* kAlignmentTop;
    static const char* kAlignmentBottom;
    static const char* kAlignmentLeft;
    static const char* kAlignmentRight;

    // Strings used in tiling properties.
    static const char* kTilingNoRepeat;
    static const char* kTilingRepeatX;
    static const char* kTilingRepeatY;
    static const char* kTilingRepeat;

    static const char* kDefaultThemeID;

    // Returns true if the image is themeable.  Safe to call on any thread.
    static bool IsThemeableImage(int resource_id);

    BrowserThemeProvider();
    virtual ~BrowserThemeProvider();

    enum
    {
        COLOR_FRAME,
        COLOR_FRAME_INACTIVE,
        COLOR_FRAME_INCOGNITO,
        COLOR_FRAME_INCOGNITO_INACTIVE,
        COLOR_TOOLBAR,
        COLOR_TAB_TEXT,
        COLOR_BACKGROUND_TAB_TEXT,
        COLOR_BOOKMARK_TEXT,
        COLOR_NTP_BACKGROUND,
        COLOR_NTP_TEXT,
        COLOR_NTP_LINK,
        COLOR_NTP_LINK_UNDERLINE,
        COLOR_NTP_HEADER,
        COLOR_NTP_SECTION,
        COLOR_NTP_SECTION_TEXT,
        COLOR_NTP_SECTION_LINK,
        COLOR_NTP_SECTION_LINK_UNDERLINE,
        COLOR_CONTROL_BACKGROUND,
        COLOR_BUTTON_BACKGROUND,

        // These colors don't have constant default values. They are derived from
        // the runtime value of other colors.
        COLOR_NTP_SECTION_HEADER_TEXT,
        COLOR_NTP_SECTION_HEADER_TEXT_HOVER,
        COLOR_NTP_SECTION_HEADER_RULE,
        COLOR_NTP_SECTION_HEADER_RULE_LIGHT,
        COLOR_NTP_TEXT_LIGHT,

        TINT_BUTTONS,
        TINT_FRAME,
        TINT_FRAME_INACTIVE,
        TINT_FRAME_INCOGNITO,
        TINT_FRAME_INCOGNITO_INACTIVE,
        TINT_BACKGROUND_TAB,
        NTP_BACKGROUND_ALIGNMENT,
        NTP_BACKGROUND_TILING,
        NTP_LOGO_ALTERNATE
    };

    // A bitfield mask for alignments.
    typedef enum
    {
        ALIGN_CENTER = 0x0,
        ALIGN_LEFT = 0x1,
        ALIGN_TOP = 0x2,
        ALIGN_RIGHT = 0x4,
        ALIGN_BOTTOM = 0x8,
    } AlignmentMasks;

    // Background tiling choices.
    typedef enum
    {
        NO_REPEAT = 0,
        REPEAT_X = 1,
        REPEAT_Y = 2,
        REPEAT = 3
    } Tiling;

    // ThemeProvider implementation.
    virtual void Init(Profile* profile);
    virtual SkBitmap* GetBitmapNamed(int id) const;
    virtual SkColor GetColor(int id) const;
    virtual bool GetDisplayProperty(int id, int* result) const;
    virtual bool ShouldUseNativeFrame() const;
    virtual bool HasCustomImage(int id) const;
    virtual base::RefCountedMemory* GetRawData(int id) const;

    // Set the current theme to the theme defined in |extension|.
    virtual void SetTheme(const Extension* extension);

    // Reset the theme to default.
    virtual void UseDefaultTheme();

    // Set the current theme to the native theme. On some platforms, the native
    // theme is the default theme.
    virtual void SetNativeTheme();

    // Whether we're using the chrome default theme. Virtual so linux can check
    // if we're using the GTK theme.
    virtual bool UsingDefaultTheme();

    // Convert a bitfield alignment into a string like "top left". Public so that
    // it can be used to generate CSS values. Takes a bitfield of AlignmentMasks.
    static std::string AlignmentToString(int alignment);

    // Parse alignments from something like "top left" into a bitfield of
    // AlignmentMasks
    static int StringToAlignment(const std::string& alignment);

    // Convert a tiling value into a string like "no-repeat". Public
    // so that it can be used to generate CSS values. Takes a Tiling.
    static std::string TilingToString(int tiling);

    // Parse tiling values from something like "no-repeat" into a Tiling value.
    static int StringToTiling(const std::string& tiling);

    // Returns the default tint for the given tint |id| TINT_* enum value.
    static gfx::HSL GetDefaultTint(int id);

    // Returns the default color for the given color |id| COLOR_* enum value.
    static SkColor GetDefaultColor(int id);

    // Returns true and sets |result| to the requested default property, if |id|
    // is valid.
    static bool GetDefaultDisplayProperty(int id, int* result);

    // Returns the set of IDR_* resources that should be tinted.
    static const std::set<int>& GetTintableToolbarButtons();

    // Save the images to be written to disk, mapping file path to id.
    typedef std::map<FilePath, int> ImagesDiskCache;

protected:
    // Get the specified tint - |id| is one of the TINT_* enum values.
    gfx::HSL GetTint(int id) const;

    // Clears all the override fields and saves the dictionary.
    virtual void ClearAllThemeData();

    // Load theme data from preferences.
    virtual void LoadThemePrefs();

    // Let all the browser views know that themes have changed.
    // extension is NULL iff the theme is being set to the
    // default/system theme.
    virtual void NotifyThemeChanged(const Extension* extension);

    // Clears the platform-specific caches. Do not call directly; it's called
    // from ClearCaches().
    virtual void FreePlatformCaches();

    Profile* profile() { return profile_; }

private:
    friend class BrowserThemeProviderTest;

    // Saves the filename of the cached theme pack.
    void SavePackName(const FilePath& pack_path);

    // Save the id of the last theme installed.
    void SaveThemeID(const std::string& id);

    // Implementation of SetTheme() (and the fallback from LoadThemePrefs() in
    // case we don't have a theme pack).
    void BuildFromExtension(const Extension* extension);

    ResourceBundle& rb_;
    Profile* profile_;

    scoped_refptr<BrowserThemePack> theme_pack_;

    DISALLOW_COPY_AND_ASSIGN(BrowserThemeProvider);
};

#endif //__wan_chrome_theme_browser_theme_provider_h__