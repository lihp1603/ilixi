/*
 Copyright 2010-2012 Tarik Sekmen.

 All Rights Reserved.

 Written by Tarik Sekmen <tarik@ilixi.org>.

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

#ifndef ILIXI_COMPOSITOR_H_
#define ILIXI_COMPOSITOR_H_

#include <compositor/ApplicationManager.h>
#include <compositor/AppView.h>
#include <compositor/CompositorComponent.h>
#include <compositor/OSKComponent.h>
#include <compositor/SoundComponent.h>
#include <compositor/Switcher.h>
#include <lib/FPSCalculator.h>
#include <ui/Application.h>

namespace ilixi
{
class Label;
/*!
 * Main compositor application.
 */
class ILXCompositor : public Application
{
    friend class ApplicationManager;
    friend class CompositorComponent;
    friend class NotificationManager;
    friend class OSKComponent;
    friend class AppView;
    friend class Notification;

public:
    /*!
     * Constructor.
     *
     * Creates ApplicationManager and Coma components.
     */
    ILXCompositor(int argc, char* argv[]);

    /*!
     * Destructor.
     *
     * Deletes all components.
     */
    virtual
    ~ILXCompositor();

    /*!
     * Sets a new switcher and removes old one.
     */
    void
    setSwitcher(Switcher* switcher);

    /*!
     * Returns the application manager instance.
     */
    ApplicationManager*
    appMan() const;

    /*!
     * Hides current application and makes given instance visible.
     */
    void
    showInstance(AppInstance* instance);

    /*!
     * Shows launcher screen and hides current application.
     *
     * @param show launcher if true, hide otherwise.
     */
    void
    toggleLauncher(bool show);

    /*!
     * Shows application switcher.
     *
     * @param show switcher if true, hide otherwise.
     */
    void
    toggleSwitcher(bool show);

    /*!
     * Hides or shows OSK.
     */
    void
    toggleOSK(bool show);

    /*!
     * Terminates client application.
     */
    void
    killApp(AppInstance* instance);

protected:
    /*!
     * Shows OSK and centers given rect at top.
     */
    void
    showOSK(DFBRectangle rect, pid_t process);

    /*!
     * Sends given key to current application instance.
     */
    void
    sendOSKInput(uint32_t key);

    void
    showSound(bool show);

    void
    showDash(bool show);

    void
    setLayerOpacity(u8 opacity);

    /*!
     * Just updates FPS.
     */
    void
    compose(const PaintEvent& event);

    void
    setOSKGeometry(const Rectangle& rect);

    void
    setAppGeometry(const Rectangle& rect);

    void
    setBarGeometry(const Rectangle& rect);

    void
    setSwitcherGeometry(const Rectangle& rect);

private:
    //! Application manager instance.
    ApplicationManager* _appMan;
    //! Current application instance.
    AppInstance* _currentApp;
    //! Previous application instance.
    AppInstance* _previousApp;

    //! Switcher instance.
    Switcher* _switcher;

    //! FPS label
    Label* _fpsLabel;
    //! FPS calculator refreshed with each paintEvent.
    FPSCalculator* _fps;

    //! CompositorComponent instance.
    CompositorComponent* _compComp;
    //! SoundComponent instance.
    SoundComponent* _soundComp;
    //! OSKComponent instance.
    OSKComponent* _oskComp;

    //! Launcher instance.
    AppInstance* _home;
    //! StatusBar instance.
    AppInstance* _statusBar;
    //! OSK instance.
    AppInstance* _osk;
    //! Dashboard instance.
    AppInstance* _dash;
    //! SoundMixer instance.
    AppInstance* _mixer;

    //! PID of application which requested OSK input.
    pid_t _oskTargetPID;
    //! Bounding rectangle around current OSK input target.
    Rectangle _oskTargetRect;

    //! This enum specifies a few window compositing related events.
    enum CompositorEventType
    {
        CET_Add,        //!< Window added
        CET_Remove,     //!< Window removed
        CET_Config,     //!< Window configured
        CET_Focus,      //!< Window focused
        CET_Restack,    //!< Window restack
        CET_State,      //!< Window state
        CET_Quit,       //!< Application terminated
        CET_Term,       //!< Terminate application.
        CET_Crash       //!< Application crashed.
    };

    //! This struct specifies a CompositorEvent.
    /*!
     * These events are pushed to compositor application's event buffer.
     */
    struct CompositorEvent
    {
        DFBEventClass clazz;
        CompositorEventType type;
    };

    //! This struct is used to carry data with compositor events.
    struct CompositorEventData
    {
        CompositorEventData()
                : instance(NULL),
                  windowID(0),
                  reconfig(0)
        {
        }

        AppInstance* instance;
        DFBWindowID windowID;
        SaWManWindowReconfig* reconfig;
    };

    //! This struct is used to store settings for compositor and its components.
    struct CompositorSettings
    {
        CompositorSettings()
                : clickFocus(true),
                  animations(true),
                  hideAnimProps(AppView::AnimatedProperty(AppView::Opacity | AppView::Zoom)),
                  showAnimProps(AppView::Opacity),
                  durationShow(300),
                  durationHide(500),
                  filter(),
                  notificationTimeout(5000)
        {
        }

        bool clickFocus;                            //!< Click to focus application in foreground.
        bool animations;
        AppView::AnimatedProperty hideAnimProps;    //!< Animated properties for hiding application.
        AppView::AnimatedProperty showAnimProps;    //!< Animated properties for visible application.
        unsigned int durationShow;                  //!< Animation duration for show.
        unsigned int durationHide;                  //!< Animation duration for hide.
        DFBConvolutionFilter filter;                //!< Convolution filter.
        unsigned int notificationTimeout;           //!< Notification will hide itself after timeout(ms).
    };

    //! This property is used by compositor components.
    CompositorSettings settings;

    Rectangle _oskGeometry;
    Rectangle _appGeometry;
    Rectangle _barGeometry;

    //! Starts auto-start applications using application manager.
    void
    onVisible();

    //! Updates fps label text.
    void
    onFPSUpdate(float fps);

    //! Returns a window on ui layer given a DFBWindowID if found, else return NULL.
    IDirectFBWindow*
    getWindow(DFBWindowID id);

    void
    addWindow(AppInstance* instance, const SaWManWindowInfo* info);

    void
    removeWindow(AppInstance* instance, const SaWManWindowInfo* info);

    void
    configWindow(AppInstance* instance, SaWManWindowReconfig *reconfig, const SaWManWindowInfo* info);

    void
    restackWindow(AppInstance* instance, const SaWManWindowInfo* info, int order, DFBWindowID other);

    void
    stateWindow(DFBWindowID id, const DFBWindowState* state);

    void
    focusWindow(DFBWindowID id);

    void
    processRemoved(AppInstance* instance);

    void
    processTerminated(AppInstance* instance);

    void
    updateCompositorGeometry();

    virtual void
    handleUserEvent(const DFBUserEvent& event);

    virtual bool
    windowPreEventFilter(const DFBWindowEvent& event);
};

} /* namespace ilixi */
#endif /* ILIXI_COMPOSITOR_H_ */