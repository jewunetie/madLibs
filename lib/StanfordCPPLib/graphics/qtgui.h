/*
 * File: qtgui.h
 * -------------
 *
 * @author Marty Stepp
 * @version 2018/09/09
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to qtgui.h
 * @version 2018/07/03
 * - initial version
 */

#ifndef _qtgui_h
#define _qtgui_h

// signal that GUI system is in use (so it will be initialized)
#ifndef INTERNAL_INCLUDE
#define SPL_QT_GUI_IN_USE 1
#endif // INTERNAL_INCLUDE

#include <functional>
#include <string>
#include <QApplication>
#include <QObject>
#include <QReadWriteLock>
#include "geventqueue.h"
#include "gthread.h"
#include "gtypes.h"
#include "queue.h"

class GWindow;

class QSPLApplication : public QApplication {
public:
    QSPLApplication(int& argc, char *argv[]);
    bool notify(QObject* receiver, QEvent* e) Q_DECL_OVERRIDE;
};


/**
 * This class contains functionality for initializing and managing the overall
 * Qt-based GUI system of the library.
 * Students and clients generally do not need to interact with this class
 * directly.
 * @private
 */
class QtGui : public QObject {
    Q_OBJECT

public:
    /**
     * Shuts down the Qt graphics system.
     */
    void exitGraphics(int exitCode = 0);

    /**
     * Returns the count of arguments passed to the main() function.
     * The string arguments themselves can be examined using getArgv.
     */
    int getArgc() const;

    /**
     * Returns the string arguments passed to the main() function.
     * The number of arguments is found using getArgc.
     */
    char** getArgv() const;

    /**
     * Initializes the Qt graphics subsystem if it is not initialized already.
     * Clients do not need to call this directly because it is called
     * automatically by the library before main() runs.
     */
    void initializeQt();

    /**
     * Returns a pointer to the QtGui object for the graphical library.
     */
    static QtGui* instance();

    /**
     * Sets the argc and argv values before main is run.
     */
    void setArgs(int argc, char** argv);

    /**
     * Runs an event loop that calls the given function that returns an int.
     * The event loop will block until the graphical program is done running.
     * If the exitAfter argument is true, calls exitGraphics after the event
     * loop is finished.
     */
    void startBackgroundEventLoop(GThunkInt mainFunc, bool exitAfter = true);

    /**
     * Runs an event loop that calls the given function that returns void.
     * The event loop will block until the graphical program is done running.
     * If the exitAfter argument is true, calls exitGraphics after the event
     * loop is finished.
     */
    void startBackgroundEventLoopVoid(GThunk mainFunc, bool exitAfter = true);
    void startEventLoop(bool exitAfter = true);

public slots:
    /**
     * A slot used by the Qt event queue.
     */
    void mySlot();

signals:
    /**
     * A signal emitted by the GUI.
     */
    void mySignal();

private:
    Q_DISABLE_COPY(QtGui)

    QtGui();   // forbid construction

    /**
     * Returns a pointer to the overall Qt application.
     * This will be null if the application has not yet been initialized.
     */
    QSPLApplication* getApplication();

    bool _initialized;
    int _argc;
    char** _argv;

    static QSPLApplication* _app;
    static QThread* _qtMainThread;
    static GStudentThread* _studentThread;
    static QtGui* _instance;

    friend class GEventQueue;
    friend class GStudentThread;
    friend class GWindow;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qtgui_h

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE
