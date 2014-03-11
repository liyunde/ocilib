/*
    +-----------------------------------------------------------------------------------------+
    |                                                                                         |
    |                                                                                         |
    |                          OCILIB ++ - C++ wrapper around OCILIB                          |
    |                                                                                         |
    |                                (C Wrapper for Oracle OCI)                               |
    |                                                                                         |
    |                              Website : http://www.ocilib.net                            |
    |                                                                                         |
    |             Copyright (c) 2007-2014 Vincent ROGIER <vince.rogier@ocilib.net>            |
    |                                                                                         |
    +-----------------------------------------------------------------------------------------+
    |                                                                                         |
    |             This library is free software; you can redistribute it and/or               |
    |             modify it under the terms of the GNU Lesser General Public                  |
    |             License as published by the Free Software Foundation; either                |
    |             version 2 of the License, or (at your option) any later version.            |
    |                                                                                         |
    |             This library is distributed in the hope that it will be useful,             |
    |             but WITHOUT ANY WARRANTY; without even the implied warranty of              |
    |             MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU           |
    |             Lesser General Public License for more details.                             |
    |                                                                                         |
    |             You should have received a copy of the GNU Lesser General Public            |
    |             License along with this library; if not, write to the Free                  |
    |             Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.          |
    |                                                                                         |
    +-----------------------------------------------------------------------------------------+

    +-----------------------------------------------------------------------------------------+
    |                                     IMPORTANT NOTICE                                    |
    +-----------------------------------------------------------------------------------------+
    |                                                                                         |
    |            This C++ header defines C++ wrapper classes around the OCILIB C API          |
    |            It requires a compatible version of OCILIB                                   |
    +-----------------------------------------------------------------------------------------+

 */

/* --------------------------------------------------------------------------------------------- *
 * $Id: ocilib.hpp, Vincent Rogier $
 * --------------------------------------------------------------------------------------------- */

#ifndef OCILIBCPP_H_INCLUDED
#define OCILIBCPP_H_INCLUDED

/* includes */

#include <string>
#include <list>
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>

#include "ocilib.h"

/**
 * @namespace ocilib
 * @brief
 */
namespace ocilib
{

/**
 * @defgroup OcilibCppApi C++ API
 * @{
 */

/**
 * @defgroup OcilibCppApiOverview Overview
 * @{
 * OCILIB ++ is a C++ API for Oracle:
 *  - Based on STL paradigms (templates, stack objects, ...)
 *  - Based on design patterns (reference counting, smart pointers, proxyies, singleton, proxies, ...)
 *  - No dynamic object allocation
 *  - Implemented as a small set of header files, no library compilation needed
 *  - Designed on top of OCILIB C API
 *  - Full C API ported  to C++
 *  - The only dependences are : STL and OCILIB C API
 *
 * @note
 *  - OCILIB++ wraps the OCILIB C API into C++ objects. 
 *  - Basically, each C OCILIB object handle has its C++ class counter parts.
 *  - The whole OCILIB C Documentation (concepts, use cases, features and functionalities) is still valid for OCILIB++
 *  - Most of the OCILIB++ classes and functions documentation refer to the C documentation
 *
 * @}
 */

/**
 * @defgroup OcilibCppApiTypes Types
 * @{
 */

/**
 * @typedef ostring
 * @brief
 * string class wrapping the OCILIB otext * type and OTEXT() macros (see @ref OcilibCApiSupportedCharsets)
 */

typedef std::basic_string<otext, std::char_traits<otext>, std::allocator<otext> > ostring;

/**
 * @typedef BufferPointer
 * @brief
 * Alias for the generic void pointer
 */

typedef void *              BufferPointer;

/**
 * @typedef UnknownHandle
 * @brief
 * Alias used for manipulating unknown handle types
 */

typedef void *              UnknownHandle;

/**
 * @typedef CallbackPointer
 * @brief
 * Alias used for storing user callback method pointers
 */

typedef void *              CallbackPointer;

/**
 * @typedef ThreadHandle
 * @brief
 * Alias for an OCI_Thread pointer
 */

typedef OCI_Thread *        ThreadHandle;

/**
 * @typedef MutexHandle
 * @brief
 * Alias for an OCI_Mutex pointer
 */

typedef OCI_Mutex *         MutexHandle ;

/* Including core classes  */

#include "ocilib_priv.hpp"

/**
 * @typedef HAHandlerProc
 *
 * @brief
 *
 */
typedef POCI_THREAD         ThreadProc;

/**
 * @typedef HAHandlerProc
 *
 * @brief
 *
 */
typedef POCI_THREADKEYDEST  ThreadKeyFreeProc;

/**
 * @typedef HAHandlerProc
 *
 * @brief
 *
 */
typedef void (*HAHandlerProc) (Connection &con, unsigned int source, unsigned int event, Timestamp  &time);

/**
 * @typedef TAFHandlerProc
 *
 * @brief
 *
 */
typedef unsigned int (*TAFHandlerProc) (Connection &con, unsigned int type, unsigned int event);

/**
 * @typedef NotifyHandlerProc
 *
 * @brief
 *
 */
typedef void (*NotifyHandlerProc) (Event &evt);

/**
 * @typedef NotifyHandlerProc
 *
 * @brief
 *
 */
typedef void (*NotifyAQHandlerProc) (Dequeue &dequeue);

/**
 * @brief
 * Exception class handling all OCILIB erors
 *
 */

class Exception : public HandleHolder<OCI_Error *>
{
    friend void ocilib::Check();
    friend class Statement;

public:

    /**
     * @brief 
     * Type of Exception
     *
     */
    enum ExceptionType
    {
        /** Unknown exception type */
        Unknown = OCI_UNKNOWN,      
        /** Exception caused by an Oracle error */
        Oracle  = OCI_ERR_ORACLE,  
        /** Exception caused by an Ocilib error */
        Ocilib  = OCI_ERR_OCILIB,  
        /** Exception caused by an Oracle Warning */
        Warning = OCI_ERR_WARNING   
    };

    /**
     * @brief
     * Retrieve the error message
     *
     */
    ostring GetMessage() const;

    /**
     *
     * @brief 
     * Return the Exception type
     *
     */
    ExceptionType GetType() const;

    /**
     * @brief
     * Return the Oracle error code
     *
     */
    int GetOracleErrorCode() const;

    /**
     * @brief
     * Return the OCILIB error code
     *
     */
    int GetInternalErrorCode() const;

    /**
     * @brief
     * Return the statement within the error occurred
     *
     */
    Statement GetStatement() const;

    /**
     * @brief
     * Return the connection within the error occurred
     *
     */
    Connection GetConnection() const;

    /**
     * @brief
     * Return the row index which caused an error during statement execution
     *
     * @warning
     * Row index start at 1.
     *
     * @return
     *  - 0 if the error is not related to array DML 
     *  - otherwise the index of the given row which caused the error
     *
     */
    unsigned int GetRow() const;

private:

    Exception();
    Exception(OCI_Error *err);
};

/**
 * @brief
 * Static class in charge of library initialization / cleanup
 *
 */
class Environment
{
    friend class Connection;
    friend class Subscription;
    friend class Dequeue;
    template<class THandleType>
    friend class HandleHolder;

public:

    /**
     * @brief 
     * Type of Exception
     *
     */
    enum EnvMode
    {
        /** Default mode */
        EnvDefault = OCI_ENV_DEFAULT,      
        /** Enable support for multithreading */
        EnvThreaded  = OCI_ENV_THREADED,  
        /** Enable support for events related to subscriptions, HA and AQ notifications */
        EnvEvents = OCI_ENV_EVENTS   
    };
    
    /**
     * @brief 
     * Type of OCI libraries import
     *
     */
    enum ImportMode
    {
        /** OCI libraires are linked at compile time */
        ImportLinkage = OCI_IMPORT_MODE_LINKAGE,      
       /** OCI libraires are dynamically loaded at runtime */
        ImportRuntime  = OCI_IMPORT_MODE_RUNTIME  
    };

    /**
     * @brief 
     * Type of Environment charset
     *
     */
    enum Charset
    {
        /** Environment is Ansi string or UTF8 string */
        CharsetAnsi = OCI_CHAR_ANSI,      
        /** Environment is Unicode using wide character string  */
        CharsetWide  = OCI_CHAR_WIDE  
    };

    /**
     * @brief 
     * Type of sessions
     *
     */
    enum SessionMode
    {
        /** Default session mode */
        SessionDefault = OCI_SESSION_DEFAULT,      
        /**  */
        SessionXa  = OCI_SESSION_XA,  
        /**  */
        SessionSysDba = OCI_SESSION_SYSDBA,   
        /**  */
        SessionSysOper = OCI_SESSION_SYSOPER
    };

    /**
     * @brief 
    * Oracle instance start modes
     *
     */
    enum StartMode
    {
        /** Start the instance wihtout mouting and opening it */
        StartOnly = OCI_DB_SPM_START,      
        /** Mount (only) the instance */
        StartMount  = OCI_DB_SPM_MOUNT,  
        /** Open (only)  the instance */
        StartOpen = OCI_DB_SPM_OPEN,   
        /** Start, mount and open the instance */
        StartFull = OCI_DB_SPM_FULL
    };

    /**
     * @brief 
     * Oracle instance start flags 
     *
     */
    enum StartFlag
    {
        /** Default start flags */
        StartDefault = OCI_DB_SPF_DEFAULT,      
        /** Shuts down a running instance (if needed) using ABORT command and starts a new instance */
        StartForce  = OCI_DB_SPF_FORCE,  
        /** Allows database access only to users with both CREATE SESSION and RESTRICTED SESSION privileges */
        StartRestrict = OCI_DB_SPF_RESTRICT   
    };


    /**
     * @brief 
    * Oracle instance shutdown modes
     *
     */
    enum ShutdownMode
    {
        /** Shutdown the instance */
        ShutdownOnly = OCI_DB_SDM_SHUTDOWN,      
        /** Close (only) the instance */
        ShutdownClose  = OCI_DB_SDM_CLOSE,  
        /** Dismount (only)  the instance */
        ShutdownDismount = OCI_DB_SDM_DISMOUNT,   
        /** Shutdown, close and dismount the instance */
        ShutdonwFull = OCI_DB_SDM_FULL
    };

    /**
     * @brief 
     * Oracle instance shutdown flags 
     *
     */
    enum ShutdownFlag
    {
        /**  - Further connects are prohibited.
          *  - Waits for users to disconnect from the database */
        ShutdowntDefault = OCI_DB_SDF_DEFAULT,      
        /**  - Further connects are prohibited
          *  - No new transactions are allowed. */
        ShutdowTrans  = OCI_DB_SDF_TRANS,  
        /**  - Further connects are prohibited
          *  - No new transactions are allowed.
          *  - Waits for active transactions to complete */
        ShutdownTransLocal = OCI_DB_SDF_TRANS_LOCAL,
        /**  - Does not wait for current calls to complete or users to disconnect from the database.
          *  - All uncommitted transactions are terminated and rolled back */
        ShutdownImmediate = OCI_DB_SDF_IMMEDIATE,      
        /**  - Does not wait for current calls to complete or users to disconnect from the database.
          *  - All uncommitted transactions are terminated and are not rolled back.
          *  - This is the fastest possible way to shut down the database, but the next
          *    database startup may require instance recovery.
          *  - Therefore, this option should be used only in unusual circumstances */
        ShutdownAbort = OCI_DB_SDF_ABORT      
    };
 
    /**
     * @brief
     * Initialize the OCILIB environment
     *
     * @param mode         - Environment mode
     * @param libpath      - Oracle shared library path (optional)
     *
     * @note
     * This function must be called before any other OCILIB library function.
     * 
     * @warning
     * It should be called <b>ONCE</b> per application
     *
     * @warning
     * - The parameter 'libpath' is only used if OCILIB has been built with the option OCI_IMPORT_RUNTIME
     * - If the parameter 'lib_path' is NULL, the Oracle library is loaded from system environment variables
     *
     */
    static void Initialize(Environment::EnvMode mode = EnvDefault, ostring libpath = OTEXT(""));
    
    /**
     * @brief
     * Clean up all resources allocated by the environment
     *
     * @note
     * This function must be the last OCILIB library function call.
     * - It deallocates objects not explicitly freed by the program (connections, statements, ...)
     * - It unloads the Oracle shared library if it has been dynamically loaded
     *
     * @warning
     * It should be called <b>ONCE</b> per application
     *
     */    
    static void Cleanup();

    /**
     * @brief
     * Return the Enviroment mode flags
     *
     * @note
     * It returns the value of the parameter 'mode' passed to Initialize()
     *
     */
    static Environment::EnvMode GetMode();

    /**
     * @brief
     * Return the Oracle shared library import mode
     *
     */
    static Environment::ImportMode GetImportMode();

    /**
     * @brief
     * Return the OCILIB charset type
     * 
     */
    static Environment::Charset GetCharset();

    /**
     * @brief
     * Return the version of OCI used for compiling OCILIB
     *
     * @note
     * - with linkage build option, the version is determined from the oci.h header through different ways
     * - with runtime loading build option, the version is set to the highest version
     *   of OCI needed by OCILIB, not necessarily the real OCI version
     *
     */
    static unsigned int GetCompileVersion();

    /**
     * @brief
     * Return the version of OCI used at runtime
     *
     * @note
     * - with linkage build option, the version is determined from the oci.h header through different ways
     * - with runtime loading build option, the version determined from the symbols dynamically loaded.
     *
     */
    static unsigned int GetRuntimeVersion();

    /**
     * @brief
     * Enable or disable Oracle warning notifications
     *
     * @param value  - enable/disable warnings
     *
     * @note
     * Default value is false
     *
     */
    static void EnableWarnings(bool value);

    /**
     * @brief
     * Start a database instance
     *
     * @param db          - Oracle Service Name
     * @param user        - Oracle User name
     * @param pwd         - Oracle User password
     * @param startFlag   - Start flags
     * @param startMode   - Start mode
     * @param sessionMode - Session mode
     * @param spfile      - Client-side spfile to start up the database (optionnal)
     *
     * @note
     * The only valid modes for sessionMode are SessionSysDba and SessionSysOper.
     *
     * @note
     * Start modes and flags parameters values can be combined.
     *
     * @note
     * External credentials are supported by supplying a empty strings for the 'user' and 'pwd' parameters
     * If the param 'db' is empty then a connection to the default local DB is done.
     *
     * @note
     * If the client side spfile is not provided, the database is started with its server-side spfile.
     *
     */
    static void StartDatabase(ostring db, ostring user, ostring pwd,
                              Environment::StartFlag startFlag,
                              Environment::StartMode startMode,
                              Environment::SessionMode sessionMode = SessionSysDba,
                              ostring spfile = OTEXT(""));

    /**
     * @brief
     * Shutdown a database instance
     *
     * @param db            - Oracle Service Name
     * @param user          - Oracle User name
     * @param pwd           - Oracle User password
     * @param shutdownFlag  - Shutdown flag
     * @param shutdownMode  - Shutdown mode
     * @param sessionMode   - Session mode
     *
     * @note
     * The only valid modes for sessionMode are SessionSysDba and SessionSysOper.
     *
     * @note
     * Shutdown modes values can be combined.
     *
     * @note
     * Shutdown flag values are exclusive.
     *
     * @note
     * External credentials are supported by supplying a empty strings for the 'user' and 'pwd' parameters
     * If the param 'db' is empty then a connection to the default local DB is done.
     *
     */
    static void ShutdownDatabase(ostring db, ostring user, ostring pwd, 
                                 Environment::ShutdownFlag shutdownFlag,
                                 Environment::ShutdownMode shutdownMode,
                                 Environment::SessionMode sessionMode = SessionSysDba);

    /**
     * @brief
     * Change the password of the given user on the given database
     *
     * @param db      - Oracle Service Name
     * @param user    - Oracle User name
     * @param pwd     - Oracle User password
     * @param newPwd  - Oracle User New password
     *
     */
    static void ChangeUserPassword(ostring db, ostring user, ostring pwd, ostring newPwd);

    /**
     * @brief
     * Set the High availabality (HA) user handler
     *
     * @param handler - Pointer to HA handler procedure
     *
     * @note
     * See POCI_HA_HANDLER documentation for more details
     *
     * @note
     * EnvMode::Events flag must be passed to Initialize() to be able to use HA events
     *
     * @warning
     * This call is supported from Oracle 10gR2.
     *
     */
    static void SetHAHandler(HAHandlerProc handler);

private:

    static void HAHandler(OCI_Connection *pConnection, unsigned int source, unsigned int event, OCI_Timestamp  *pTimestamp);
    static unsigned int TAFHandler(OCI_Connection *pConnection, unsigned int type, unsigned int event);
    static void NotifyHandler(OCI_Event *pEvent);
    static void NotifyHandlerAQ(OCI_Dequeue *pDequeue);

    typedef ConcurrentPool<UnknownHandle, Handle *> HandlePool;
    typedef ConcurrentPool<UnknownHandle, CallbackPointer> CallbackPool;

    class EnvironmentHandle : public HandleHolder<UnknownHandle>
    {
        friend class Connection;

    public:

        HandlePool   Handles;
        CallbackPool Callbacks;
        unsigned int Mode;

        void Initialize(UnknownHandle pEnv, unsigned int envMode);
        void Finalize();
    };

    static EnvironmentHandle& GetEnvironmentHandle();
};

/**
 * @brief
 * static class allowing to manipulate threads
 *
 * This class wraps methods manipulating OCILIB OCI_Thread objects
 *
 * @note
 * See @ref OcilibCApiThreading for more details on Oracle multithreading support
 *
 */
class Thread
{
public:

    /**
     * @brief
     * Create a Thread
     *
     * @return
     * Thread handle on success or NULL on failure
     *
     */
    static ThreadHandle Create();

    /**
     * @brief
     * Destroy a thread
     *
     * @param handle - Thread handle
     *
     */
    static void Destroy(ThreadHandle handle);

    /**
     * @brief
     * Execute the given routine within the given thread
     *
     * @param handle - Thread handle
     * @param func   - routine to execute
     * @param args   - parameter to pass to the routine
     *
     */
    static void Run(ThreadHandle handle, ThreadProc func, void *args);

    /**
     * @brief
     * Join the given thread
     *
     * @param handle - Thread handle
     *
     * @note
     * This function waits for the given thread to finish
     *
     */
    static void Join(ThreadHandle handle);
};

/**
 * @brief
 * Static class allowing managing mutexes
 *
 * This class wraps methods manipulating OCILIB OCI_Mutex objects
 *
 * @note
 * See @ref OcilibCApiThreading for more details on Oracle multithreading support
 *
 */
class Mutex
{
public:

    /**
     * @brief
     * Create a Mutex handle
     *
     * @return
     * Mutex handle on success or NULL on failure
     *
     */
    static MutexHandle Create();

    /**
     * @brief
     * Destroy a mutex handle
     *
     * @param handle - Mutex handle
     *
     */
    static void Destroy(MutexHandle handle);

    /**
     * @brief
     * Acquire a mutex lock
     *
     * @param handle - Mutex handle
     *
     */
    static void Acquire(MutexHandle handle);

    /**
     * @brief
     * Release a mutex lock
     *
     * @param handle - Mutex handle
     *
     */
    static void Release(MutexHandle handle);
};

/**
 * @brief
 * Static class allowing to set/get thread local storage (TLS) values for a given unique key
 *
 * This class wraps methods manipulating OCILIB OCI_ThreadKey objects
 *
 * @note
 * See @ref OcilibCApiThreading for more details on Oracle multithreading support
 *
 */
class ThreadKey
{
public:

    /**
     * @brief
     * Create a thread key object
     *
     * @param name     - Thread key name
     * @param freeProc - Thread key value destructor function
     *
     * @note
     * Parameter freeProc is optional. It's called when the thread terminates to allow
     * the program to deal with the thread specific value of the key
     *
     */
    static void Create(ostring name, ThreadKeyFreeProc freeProc = 0);

    /**
     * @brief
     * Set a thread key value
     *
     * @param name  - Thread key name
     * @param value - user value to set
     *
     */
    static void SetValue(ostring name, void *value);

    /**
     * @brief
     * Get a thread key value
     *
     * @param name - Thread key name
     *
     * @return
     * Thread key value on success otherwise FALSE
     *
     */
    static void * GetValue(ostring name);
};

/**
  * @brief
  * A connection or session Pool.
  *
  * @note
  * This class wraps methods manipulating OCILIB OCI_Pool objects
  *
  */
class Pool : public HandleHolder<OCI_Pool *>
{
public:

    /**
     * @brief 
     * Type of Pool
     *
     */
    enum PoolType
    {
        /** Pool of Connections */
        ConnectionPool = OCI_POOL_CONNECTION,      
        /** Pool of stateless sessions */
        SessionPool  = OCI_POOL_SESSION
    };

    /**
     * @brief
     * Default constructor
     *
     */
    Pool();

    /**
     * @brief
     * Contructor that creates an underlying pool with the given information
     *
     * @param db           - Oracle Service Name
     * @param user         - Oracle User name
     * @param pwd          - Oracle User password
     * @param poolType     - Type of pool
     * @param sessionMode  - Session mode
     * @param minSize      - minimum number of  connections/sessions that can be opened.
     * @param maxSize      - maximum number of  connections/sessions that can be opened.
     * @param increment    - next increment for connections/sessions to be opened
     *
     *  @note
     * it calls Open() with the given parameters
     *
     */
    Pool(ostring db, ostring user, ostring pwd, Pool::PoolType poolType,
         unsigned int minSize, unsigned int maxSize, unsigned int increment = 1,
         Environment::SessionMode sessionMode = Environment::SessionDefault);

    /**
     * @brief
     * Create an Oracle pool of connections or sessions
     *
     * @param db           - Oracle Service Name
     * @param user         - Oracle User name
     * @param pwd          - Oracle User password
     * @param poolType     - Type of pool
     * @param sessionMode  - Session mode
     * @param minSize      - minimum number of  connections/sessions that can be opened.
     * @param maxSize      - maximum number of  connections/sessions that can be opened.
     * @param increment    - next increment for connections/sessions to be opened
     *
     * @note
     * External credentials are supported by supplying an emtpy string for the 'user' and 'pwd' parameters
     * If the param 'db' is empty then a connection to the default local DB is done
     *
     */
    void Open(ostring db, ostring user, ostring pwd, Pool::PoolType poolType,
              unsigned int minSize, unsigned int maxSize, unsigned int increment = 1,
              Environment::SessionMode sessionMode = Environment::SessionDefault);

    /**
     * @brief
     * Destroy the current Oracle pool of connections or sessions
     *
     */
    void Close();

    /**
     * @brief
     * Get a connection from the pool
     *
     * @param sessionTag  - Session user tag string
     *
     * @par Session tagging
     *
     * Session pools have a nice feature called 'session tagging'
     * It's possible to tag a session with a string identifier
     * when the session is returned to the pool, it keeps its tags.
     * When requesting a connection from the session pool, it's
     * possible to request a session that has the given 'tag' parameter
     * If one exists, it is returned. If not and if an untagged session
     * is available, it is then returned. So check the connection tag
     * property with OCI_GetSessionTag() to find out if the returned
     * connection is tagged or not.
     *
     * This features is described in the OCI developper guide as the following :
     *
     *  "The tags provide a way for users to customize sessions in the pool.
     *   A client may get a default or untagged session from a pool, set certain
     *   attributes on the session (such as NLS settings), and return the session
     *   to the pool, labeling it with an appropriate tag.
     *   The user may request a session with the same tags in order to have a
     *   session with the same attributes"
     *
     */
    Connection GetConnection(ostring sessionTag = OTEXT(""));

    /**
     * @brief
     * Get the idle timeout for connections/sessions in the pool
     *
     * @note
     * Connections/sessions idled for more than this time value (in seconds) are terminated
     *
     * @note
     * Timeout is not available for internal pooling implementation (client < 9i)
     *
     */
    unsigned int GetTimeout() const;

    /**
     * @brief
     * Set the connections/sessions idle timeout
     *
     * @param value - Timeout value
     *
     * @note
     * connections/sessions idle for more than this time value (in seconds) are terminated
     *
     * @note
     * This call has no effect if pooling is internally implemented (client < 9i)
     *
     */
    void SetTimeout(unsigned int value);

    /**
     * @brief
     * Get the waiting mode used when no more connections/sessions are available from the pool
     *
     * @return
     * - true to wait for an available object if the pool is saturated
     * - false to not wait for an available object
     *
     */
    bool GetNoWait() const;

    /**
     * @brief
     * Set the waiting mode used when no more connections/sessions are available from the pool
     *
     * @param value - wait for object
     *
     * @note
     * For paramter value, pass :
     * - true to wait for an available object if the pool is saturated
     * - false to not wait for an available object
     *
     */
    void SetNoWait(bool value);

    /**
     * @brief
     * Return the current number of busy connections/sessions
     *
     */
    unsigned int GetBusyConnectionsCount() const;

    /**
     * @brief
     * Return the current number of opened connections/sessions
     *
     */
    unsigned int GetOpenedConnectionsCount() const;

    /**
     * @brief
     * Return the minimum number of connections/sessions that can be opened to the database
     *
     */
    unsigned int GetMinSize() const;

    /**
     * @brief
     * Return the maximum number of connections/sessions that can be opened to the database
     *
     */
    unsigned int GetMaxSize() const;
    
    /**
     * @brief
     * Return the increment for connections/sessions to be opened to the database when the pool is not full
     *
     */
    unsigned int GetIncrement() const;

    /**
     * @brief
     * Return the maximum number of statements to keep in the pool's statement cache
     *
     * @note
     * Default value is 20 (value from Oracle Documentation)
     *
     */
    unsigned int GetStatementCacheSize() const;
    
    /**
     * @brief
     * Set the maximum number of statements to keep in the pool's statement cache
     *
     * @param value - maximun number of statements in the cache
     *
     */    
    void SetStatementCacheSize(unsigned int value);
};

/**
 * @class Connection
 *
 * @brief
 * A connection or session with a specific database.
 */
class Connection : public HandleHolder<OCI_Connection *>
{
    friend class Environment;
    friend class Exception;
    friend class Statement;
    friend class Clob;
    friend class Blob;
    friend class File;
    friend class Timestamp;
    friend class Pool;
    friend class Object;
    friend class TypeInfo;
    friend class Reference;
    friend class Resultset;
    friend class Collection;
    friend class Subscription;

public:

    /**
     * @brief 
     * Type of session trace
     *
     */
    enum SessionTrace
    {
        /** Specifies the user defined identifier in the session. It's recorded in the column CLIENT_IDENTIFIER of the system view V$SESSION */
        TraceIdentity = OCI_TRC_IDENTITY,      
        /** Name of the current module in the client application. It's recorded in the column MODULE of the system view V$SESSION */
        TraceModule  = OCI_TRC_MODULE,
        /** Name of the current action within the current module. It's recorded in the column ACTION of the system view V$SESSION */
        TraceAction = OCI_TRC_ACTION,      
        /** Client application additional information. It's recorded in the column CLIENT_INFO of the system view V$SESSION */
        TraceDetail  = OCI_TRC_DETAIL
    };

    /**
     * @brief
     * Default constructor
     *
     */
    Connection();

    /**
     * @brief
     * Contructor that creates an opens an underlying DB connection with the given information
     *
     * @param db           - Oracle Service Name
     * @param user         - Oracle User name
     * @param pwd          - Oracle User password
     * @param sessionMode  - Session mode
     *
     *  @note
     * it calls Open() with the given parameters
     *
     */
    Connection(ostring db, ostring user, ostring pwd, Environment::SessionMode sessionMode = Environment::SessionDefault);

    /**
     * @brief
     * Create a physical connection to an Oracle database server   
     *
     * @param db           - Oracle Service Name
     * @param user         - Oracle User name
     * @param pwd          - Oracle User password
     * @param sessionMode  - Session mode
     * *
     * @note
     * External credentials are supported by supplying an emtpy string for the 'user' and 'pwd' parameters
     * If the param 'db' is empty then a connection to the default local DB is done
     *
     * @par Oracle XA support
     * OCILIB supports Oracle XA connectivity. In order to get a connection using
     * the XA interface :
     *   - For parameter 'db' : pass the value of the 'DB' parameter of the given
     *    XA connection string pased to the Transaction Processing Monitor (TPM)
     *   - Pass emtpy strings for 'user' and 'pwd' parameters
     *   - Use SessionMode:Xa for parameter 'sessionMode'
     *
     * @par Oracle XA Connection String
     *
     * The XA connection string used in a transaction monitor to connect to Oracle must
     * be compatible with OCILIB :
     *   - the XA parameter 'Objects' MUST be set to 'true'
     *   - If EnvMode::Threaded is passed to Environment::Initialize(), the XA parameter 'Threads' must
     *     be set to 'true', otherwise to 'false'
     *   - If EnvMode::Events is passed to Environment::Initialize(), the XA parameter 'Events' must
     *     be set to 'true', otherwise to 'false'
     *   - As Oracle does not support Unicode UTF16 characterset through the XA interface,
     *     Only OCI_CHARSET_ANSI builds of OCILIB can be used
     *   - You still can use UTF8 if the NLS_LANG environment variable is set with a valid
     *     UTF8 NLS value
     *   - DO NOT USE OCI_CHARSET_WIDE OCILIB builds with XA connections
     *
     * @note
     * On success, a local transaction is automatically created and started ONLY for regular 
     * standalone connections and connections retrieved from connection pools.
     * No transaction is created for a XA connection or q connection retrieved from session pools.
     *
     */
    void Open(ostring db, ostring user, ostring pwd,  Environment::SessionMode sessionMode = Environment::SessionDefault);
    
    /**
     * @brief
     * Close the physical connection to the DB server
     *
     */
    void Close();

    /**
     * @brief
     * Commit current pending changes
     *
     */
    void Commit();

   /**
     * @brief
     * Cancel current pending changes
     *
     */
    void Rollback();

    /**
     * @brief
     * Perform an immediate abort of any currently Oracle OCI call on the given connection
     *
     * @note
     * The current call will abort and will raise an exception
     *
     */
    void Break();

    /**
     * @brief
     * Enable or disable auto commit mode (implicit commits after every SQL execution)
     *
     * @param enabled - auto commit new status
     *
     */
    void SetAutoCommit(bool enabled);

    /**
     * @brief
     * Indicates if autocommit is currently activated
     *
     */
    bool GetAutoCommit() const;

    /**
    * @brief
    * Indiciated if the connection is still connected to the server 
    *
    * @note
    * the returned value is not realtime and is  based on client libray last heart beat status
    *
    */
    bool IsServerAlive() const;

    /**
     * @brief
     * Performs a round trip call to the server to confirm that the connection to the server is still valid.
     *
     * @warning
     * This call is supported from Oracle 10g.
     *
     */
    bool PingServer() const;

    /**
     * @brief
     * Return the name of the connected database/service name
     *
     * @note
     * The returned value is the value of the 'db' parameter of the Open() method
     *
     */
    ostring GetConnectionString() const;

    /**
     * @brief
     * Return the current logged user name
     *
     * @note
     * The returned value is the value of the 'user' parameter of the Open() method
     *
     */
    ostring GetUserName() const;

    /**
     * @brief
     * Return the current logged user password
     *
     * @note
     * The returned value is the value of the 'pwd' parameter of the Open() method
     *
     */
    ostring GetPassword() const;

    /**
     * @brief
     * Return the connected database server string version
     *
     * @note
     * The returned value is the server version banner displayed by SQL*PLUS when connected to a DB server
     *
     */
    ostring GetServerVersion() const;

    /**
     * @brief
     * Return the Oracle version supported by the connection
     *
     * @note
     * The supported version is the lower version between client and server:
     *  - OCI_UNKNOWN
     *  - OCI_8_0
     *  - OCI_8_1
     *  - OCI_9_0
     *  - OCI_9_2
     *  - OCI_10_1
     *  - OCI_10_2
     *  - OCI_11_1
     *  - OCI_11_2
     *  - OCI_12_1
     *
     */
    unsigned int GetVersion() const;

    /**
     * @brief
     * Return the major version number of the connected database server
     *
     */
    unsigned int GetServerMajorVersion() const;

    /**
     * @brief
     * Return the minor version number of the connected database server
     *
     */
    unsigned int GetServerMinorVersion() const;

    /**
     * @brief
     * Return the revision version number of the connected database server
     *
     */
    unsigned int GetServerRevisionVersion() const;

    /**
     * @brief
     * Change the password of the logged user
     *
     * @param newPwd - New password
     *
     */
    void ChangePassword(ostring newPwd);

    /**
     * @brief
     * Return the tag associated the the given connection
     *
     */
    ostring GetSessionTag() const;

    /**
     * @brief
     * Associate a tag to the given connection/session
     *
     * @param tag - user tag string
     *
     * @note
     * Use this call only for connections retrieved from a session pool
     * See Pool::GetConnection() for more details
     *
     * @note
     * To untag a session, call SetSessionTag() with an empty 'tag' parameter
     *
     */
    void SetSessionTag(ostring tag);

    /**
     * @brief
     * Return the current transaction of the connection
     *
     */
    Transaction GetTransaction() const;

    /**
     * @brief
     * Set a transaction to a connection
     *
     * @param transaction - Transaction to assign
     *
     * @note
     * The current transaction (if any) is automatically stopped but the newly assigned is not started or resumed
     * 
     * @warning
     * Do not set a transaction to a XA connection or a connection retrieved from a session pool
     *
     */
    void SetTransaction(const Transaction &transaction);

    /**
     * @brief
     * Set the date format for implicit string / date conversions
     *
     * @param format - Date format
     *
     * @note
     * Default format is 'YYYY-MM-DD' defined by the public constant OCI_STRING_FORMAT_DATE
     *
     * @note
     * Conversions are performed by Oracle builtin functions.
     * Possible values are string date formats supported by Oracle.
     * See documentation of Oracle SQL to_date() function for more details
     *
     */
    void SetDefaultDateFormat(ostring format);

    /**
     * @brief
     * Return the current date format for implicit string / date conversions
     *
     * @note
     *  See SetDefaultDateFormat() for possible values
     *
     */
    ostring  GetDefaultDateFormat() const;

    /**
     * @brief
     * Set the numeric format for implicit string / numeric conversions
     *
     * @param format - Numeric format
     *
     * @note
     * Conversions are performed by Oracle builtin functions.
     * Possible format values are the numeric formats supported by Oracle.
     * See documentation of Oracle SQL to_number() function for more details
     *
     * @note
     * Default format is 'FM99999999999999999999999999999999999990.999999999999999999999999'
     * defined by the public constant OCI_STRING_FORMAT_NUM
     * 
     * @warning
     * It does not applies to binary double and binary floats data types that
     * are converted from/to strings using the standard C library
     *
     */
    void SetDefaultNumericFormat(ostring format);

    /**
     * @brief
     * Return the current numeric format for implicit string / numeric conversions
     *
     * @note
     *  See SetDefaultNumericFormat() for possible values
     *
     */
    ostring  GetDefaultNumericFormat() const;

    /**
     * @brief
     * Enable the server output
     *
     * @param bufsize - server buffer max size (server side)
     * @param arrsize - number of lines to retrieve per server roundtrip
     * @param lnsize  - maximum size of one line
     *
     * @note
     * This call is equivalent to the command 'set serveroutput on' in SQL*PLUS
     *
     * @note
     *  - 'bufsize' minimum value is 2000, maximum 1000000 with Oracle < 10.2g and can be unlimited above
     *  -'lnsize' maximum value is 255 with Oracle < 10g R2 and 32767 above
     *
     * @warning
     * If EnableServerOutput() is not called, GetServerOutput() will return false
     *
     */
    void EnableServerOutput(unsigned int bufsize, unsigned int arrsize, unsigned int lnsize);

    /**
     * @brief
     * Disable the server output
     *
     * @note
     * After this call, GetServerOutput() will return false.
     *
     */
    void DisableServerOutput();

    /**
     * @brief
     * Retrieve one line of the server buffer
     *
     * @return
     * true if a line has been retrieved otherwise false (server buffer is empty or all lines have been retrieved)
     *
     */
    bool GetServerOutput(ostring &line) const;

    /**
     * @brief
     * Retrieve all remaining lines of the server buffer
     *
     */
    void GetServerOutput(std::vector<ostring> &lines) const;


    /**
     * @brief
     * Set tracing information for the session
     *
     * @param trace - trace type
     * @param value - trace content
     *
     * Store current trace information to the given connection handle.
     * These information:
     *
     * - is stored in the system view V$SESSION
     * - can be retrieved from the connection property of an OCI_Error handle
     *
     * system view V$SESSION
     *
     * @warning
     * The system view V$SESSION is updated on Oracle versions >= 10g
     *
     * @warning
     * Oracle limits the size of these traces content and thus OCILIB will truncate
     * the given values if needed :
     *
     * - TraceIdentity : 64 bytes
     * - TraceModule   : 48 bytes
     * - TraceAction   : 32 bytes
     * - TraceDetail   : 64 bytes
     *
     */
    void SetTrace(SessionTrace trace, ostring value);

    /**
     * @brief
     * Get the current trace for the trace type from the given connection.
     *
     * @param trace - trace type
     *
     * @note
     * See SetTrace() for more details.
     *
     */
    ostring GetTrace(SessionTrace trace) const;

    /**
     * @brief
     * Return the Oracle server database name of the connected database/service name
     *
     * @warning
     * This call is supported from Oracle 10gR2.
     *
     */
    ostring GetDatabase() const;

    /**
     * @brief
     * Return the Oracle server Instance name of the connected database/service name
     *
     * @warning
     * This call is supported from Oracle 10gR2.
     *
     */
    ostring GetInstance() const;

    /**
     * @brief
     * Return the Oracle server Service name of the connected database/service name
     *
     * @warning
     * This call is supported from Oracle 10gR2.
     *
     */
    ostring GetService() const;

    /**
     * @brief
     * Return the Oracle server Hos name of the connected database/service name
     *
     * @warning
     * This call is supported from Oracle 10gR2.
     *
     */
    ostring GetServer() const;

    /**
     * @brief
     * Return the Oracle server Domain name of the connected database/service name
     *
     * @warning
     * This call is supported from Oracle 10gR2.
     *
     */
    ostring GetDomain() const;

    /**
     * @brief
     * Return the date and time (Timestamp) server instance start of the
     *
     * @warning
     * This call is supported from Oracle 10gR2.
     *
     */
    Timestamp GetInstanceStartTime() const;

    /**
     * @brief
     * Return the maximum number of statements to keep in the statement cache
     *
     * @note
     * Default value is 20 (value from Oracle Documentation)
     *
     * @warning
     * Requires Oracle Client 9.2 or above
     *
     */
    unsigned int GetStatementCacheSize() const;

    /**
     * @brief
     * Set the maximum number of statements to keep in the statement cache
     *
     * @param value - maximum number of statements in the cache
     *
     * @warning
     * Requires Oracle Client 9.2 or above
     *
     */
    void SetStatementCacheSize(unsigned int value);

    /**
     * @brief
     * Return the default LOB prefetch buffer size for the connection
     * 
     * @warning
     * Requires Oracle Client AND Server 11gR1 or above
     *
     * @note 
     * Prefetch size is:
     * - number of bytes for BLOBs and BFILEs
     * - number of characters for CLOBs.
     *
     * @note
     * Default is 0 (prefetching disabled)
     *
     */
    unsigned int GetDefaultLobPrefetchSize() const;

    /**
     * @brief
     * Enable or disable prefetching for all LOBs fetched in the connection
     *
     * @param value - default prefetch buffer size
     *
     * @note
     * If parameter 'value':
     * - is == 0, it disables prefetching for all LOBs fetched in the connection.
     * - is >  0, it enables prefetching for all LOBs fetched in the connection 
     * and the given buffer size is used for prefetching LOBs
     *
     * @note
     * LOBs prefetching is disabled by default
     *
     * @warning
     * Requires Oracle Client AND Server 11gR1 or above.
     *
     * @note 
     * Prefetch size is:
     * - number of bytes for BLOBs and BFILEs
     * - number of characters for CLOBs.
     *
     */
    void SetDefaultLobPrefetchSize(unsigned int value);

    /**
     * @brief
     * Verifiy if the connection support TAF events
     *
     * @warning
     * This call is supported from Oracle 10gR2.
     *
     */
    bool IsTAFCapable() const;

    /**
     * @brief
     * Set the Transparent Application Failover (TAF) user handler
     *
     * @param handler - Pointer to TAF handler procedure
     *
     * @note
     * See TAFHandlerProc documentation for more details
     *
     * @warning
     * This call is supported from Oracle 10gR2.
     *
     */
    void SetTAFHandler(TAFHandlerProc handler);

    /**
     * @brief
     * Return the pointer to user data previously associated with the connection
     *
     */
    void * GetUserData();

    /**
     * @brief
     * Associate a pointer to user data to the given connection
     *
     * @param value - User data pointer
     *
     * @return
     * TRUE on success otherwise FALSE
     *
     */
    void SetUserData(void *value);

private:

    Connection(OCI_Connection *con, Handle *parent);
};

/**
 * @class Transaction
 *
 * @brief
 * Oracle Transaction object
 *
 */
class Transaction : public HandleHolder<OCI_Transaction *>
{
    friend class Connection;

public:

    /**
     * @brief 
     * Transaction mode
     *
     */
    enum TransactionMode
    {
        /** (Global) Specifies tightly coupled and migratable branch */
        TransactionNew = OCI_TRS_NEW,      
        /** (Global) Specifies a tightly coupled branch */
        TransactionTight  = OCI_TRS_TIGHT,
        /** (Global) Specifies a loosely coupled branch */
        TransactionLoose = OCI_TRS_LOOSE,      
        /** (Global and local) start a read-only transaction */
        TransactionReadOnly  = OCI_TRS_READONLY,
        /** (Global and local) start a read-write transaction */
        TransactionReadWrite = OCI_TRS_READWRITE,      
        /** (Global and local) start a serializable transaction */
        TransactionSerializable  = OCI_TRS_SERIALIZABLE
    };

    /**
     * @brief
     * Create a new global transaction or a serializable/read-only local transaction
     *
     * @param connection - Connection
     * @param timeout    - Time that a transaction stays inactive after being stopped
     * @param mode       - Transaction mode
     * @param pxid       - pointer to a global transaction identifier structure
     *
     * @note
     * For local transaction,  don't use the 'pxid' parameter
     *
     */
   Transaction(const Connection &connection, unsigned int timeout, TransactionMode mode, OCI_XID *pxid = NULL);

   /**
     * @brief
     * Prepare a global transaction validation
     *
     */
   void Prepare();

   /**
     * @brief
     * Start global transaction
     *
     */
   void Start();

    /**
     * @brief
     * Stop current global transaction
     *
     */
   void Stop();

    /**
     * @brief
     * Resume a stopped global transaction
     *
     */
   void Resume();

    /**
     * @brief
     * Cancel the prepared global transaction validation
     *
     */
   void Forget();

    /**
     * @brief
     * Return the transaction mode.
     *
     * @note:
     * see Transaction() for possible values
     *
     */
   Transaction::TransactionMode GetMode() const;

   /**
     * @brief
     * Return the transaction Timeout
     *
     */
   unsigned int GetTimeout() const;

private:

    Transaction(OCI_Transaction *trans);
};

/**
 * @class Date
 *
 * @brief
 * Object identififying the SQL datatype DATE.
 *
 */
class Date : public HandleHolder<OCI_Date *>
{
    friend class Statement;
    friend class Resultset;
    friend class BindArray;
    friend class Object;
    friend class Collection;
    friend class Message;

public:
    
    /**
     * @brief
     * Create an empty date object
     *
     */
    Date();

    /**
     * @brief
     * Assign the value of the given date
     *
     * @param other - Source Date handle
     *
     */
    void Assign(const Date& other);

    /**
     * @brief
     * Check if the given date is valid
     *
     */
    bool IsValid() const;

    /**
     * @brief
     * Return the date year value
     *
     */
    int GetYear() const;

    /**
     * @brief
     * Set the date year value
     *
     */
    void SetYear(int value);
 
    /**
     * @brief
     * Return the date month value
     *
     */
    int GetMonth() const;

    /**
     * @brief
     * Set the date month value
     *
     */
    void SetMonth(int value);

    /**
     * @brief
     * Return the date day value
     *
     */
    int GetDay() const;

    /**
     * @brief
     * Set the date day value
     *
     */
    void SetDay(int value);
 
    /**
     * @brief
     * Return the date hours value
     *
     */
    int GetHours() const;

    /**
     * @brief
     * Set the date hours value
     *
     */
    void SetHours(int value);
  
    /**
     * @brief
     * Return the date minutes value
     *
     */
    int GetMinutes() const;

    /**
     * @brief
     * Set the date minutes value
     *
     */
    void SetMinutes(int value);

    /**
     * @brief
     * Return the date seconds value
     *
     */
    int GetSeconds() const;

    /**
     * @brief
     * Set the date seconds value
     *
     */
    void SetSeconds(int value);

    /**
     * @brief
     * Return the number of days with the given date
     *
     * @param other - date to compare
     *
     */
    int DaysBetween(const Date& other) const;

    /**
     * @brief
     * Set the date part
     *
     * @param year  - Year value
     * @param month - Month value
     * @param day   - Day value
     *
     */
    void SetDate(int year, int month, int day);

    /**
     * @brief
     * Set the time part
     *
     * @param hour  - Hour value
     * @param min   - Minute value
     * @param sec   - Second value
     *
     */
    void SetTime(int hour, int min,   int sec);

    /**
     * @brief
     * Set the date and time part
     *
     * @param year  - Year value
     * @param month - Month value
     * @param day   - Day value
     * @param hour  - Hour value
     * @param min   - Minute value
     * @param sec   - Second value
     *
     */
    void SetDateTime(int year, int month, int day, int hour, int min, int sec);

    /**
     * @brief
     * Extract the date parts
     *
     * @param year  - Place holder for year value
     * @param month - Place holder for month value
     * @param day   - Place holder for day value
     *
     */
    void GetDate(int *year, int *month, int *day) const;

    /**
     * @brief
     * Extract time parts
     *
     * @param hour  - Place holder for hour value
     * @param min   - Place holder for minute value
     * @param sec   - Place holder for second value
     *
     */
    void GetTime(int *hour, int *min,   int *sec) const;

    /**
     * @brief
     * Extract the date and time parts
     *
     * @param year  - Place holder for year value
     * @param month - Place holder for month value
     * @param day   - Place holder for day value
     * @param hour  - Place holder for hour value
     * @param min   - Place holder for minute value
     * @param sec   - Place holder for second value
     *
     */
    void GetDateTime(int *year, int *month, int *day, int *hour, int *min, int *sec) const;

    /**
     * @brief
     * Add or subtract days
     *
     * @param nb   - Number of days to add/remove
     *
     */
    void AddDays(int days);

    /**
     * @brief
     * Add or subtract months
     *
     * @param nb   - Number of months to add/remove
     *
     */
    void AddMonths(int months);

    /**
     * @brief
     * Assign the current system datetime to the current date object
     *
     */
    void SysDate();

    /**
     * @brief
     *  Return the date of next day of the week, after the current date object
     *
     * @param day - Day of the week
     *
     */
    Date NextDay(ostring day) const;

    /**
     * @brief
     * Return the last day of month from the current date object
     *
     */
    Date LastDay() const;

    /**
     * @brief
     * Convert the date from one zone to another zone
     *
     * @param tzSrc - Source zone
     * @param tzDst - Destination zone
     *
     */
    void ChangeTimeZone(ostring tzSrc, ostring tzDst);


    void FromString(ostring data, ostring format = OCI_STRING_FORMAT_DATE);
    ostring ToString(ostring format = OCI_STRING_FORMAT_DATE) const;

    operator ostring() const;

    /**
     * @brief
     * Increment the date by 1 day
     *
     * @note
     * This operator overload calls AddDays()  
     *
     */
	Date& operator ++ (int);  

    /**
     * @brief
     * Decrement the date by 1 day
     *
     * @note
     * This operator overload calls AddDays()  
     *
     */
	Date& operator -- (int);  

    /**
     * @brief
     * Assign the given date object
     *
     * @note
     * This operator overload calls Assign()  
     *
     */
	Date& operator = (const Date& other);

    /**
     * @brief
     * Increment the date by the given number of days
     *
     * @note
     * This operator overload calls AddDays()  
     *
     */
	Date& operator + (int val);

    /**
     * @brief
     * Decrement the date by the given number of days
     *
     * @note
     * This operator overload calls AddDays()  
     *
     */
	Date& operator - (int val);

    /**
     * @brief
     * Increment the date by the given number of days
     *
     * @note
     * This operator overload calls AddDays()  
     *
     */
	Date& operator += (int val);

    /**
     * @brief
     * Decrement the date by the given number of days
     *
     * @note
     * This operator overload calls AddDays()  
     *
     */
	Date& operator -= (int val);

    /**
     * @brief
     * Decrement the date by the given number of days
     *
     * @note
     * This operator overload calls AddDays()  
     *
     */
	bool operator == (const Date& other) const;
	bool operator != (const Date& other) const;			
	bool operator > (const Date& other) const;
	bool operator < (const Date& other) const;
	bool operator >= (const Date& other) const;
	bool operator <= (const Date& other) const;

private:

    int Compare(const Date& other) const;

    Date(OCI_Date *pDate, Handle *parent = 0);
};

/**
 * @class Interval
 *
 * @brief
 * Object identififying the SQL datatype INTERVAL.
 *
 */
class Interval : public HandleHolder<OCI_Interval *>
{
    friend class Environment;
    friend class Statement;
    friend class Resultset;
    friend class BindArray;
    friend class Object;
    friend class Collection;

public:

    Interval(unsigned int type);

    void Assign(const Interval& other);
    int Compare(const Interval& other) const;

    unsigned int GetType() const;

    void Add(const Interval& other);
    void Substract(const Interval& other);

    bool IsValid() const;

    int GetYear() const;
    void SetYear(int value);
 
    int GetMonth() const;
    void SetMonth(int value);

    int GetDay() const;
    void SetDay(int value);
 
    int GetHours() const;
    void SetHours(int value);
  
    int GetMinutes() const;
    void SetMinutes(int value);

    int GetSeconds() const;
    void SetSeconds(int value);

    int GetMilliSeconds() const;
    void SetMilliSeconds(int value);

    void GetDaySecond(int *day, int *hour, int *min, int *sec, int *fsec) const;
    void SetDaySecond(int day, int hour, int min, int sec, int fsec);

    void GetYearMonth(int *year, int *month) const;
    void SetYearMonth(int year, int month);

    void FromTimeZone(ostring timeZone);

    void FromString(ostring data);
    ostring ToString(int leadingPrecision = 10, int fractionPrecision = 10) const;

    operator ostring() const;

private:

    Interval(OCI_Interval *pInterval, Handle *parent = 0);
};

/**
 * @class Timestamp
 *
 * @brief
 * Object identififying the SQL datatype TIMESTAMP.
 *
 */
class Timestamp : public HandleHolder<OCI_Timestamp *>
{
    friend class Environment;
    friend class Statement;
    friend class Resultset;
    friend class BindArray;
    friend class Object;
    friend class Connection;
    friend class Collection;

public:

    Timestamp(unsigned int type);

    void Assign(const Timestamp& other);
    int Compare(const Timestamp& other) const;

    unsigned int GetType() const;

    void Construct(int year, int month, int day, int hour, int min, int sec, int fsec, ostring timeZone = OTEXT(""));
    void Convert(const Timestamp& other);

    bool IsValid() const;

    int GetYear() const;
    void SetYear(int value);
 
    int GetMonth() const;
    void SetMonth(int value);

    int GetDay() const;
    void SetDay(int value);
 
    int GetHours() const;
    void SetHours(int value);
  
    int GetMinutes() const;
    void SetMinutes(int value);

    int GetSeconds() const;
    void SetSeconds(int value);

    int GetMilliSeconds() const;
    void SetMilliSeconds(int value);

    void GetDate(int *year, int *month, int *day) const;
    void GetTime(int *hour, int *min,   int *sec, int *fsec) const;
    void GetDateTime(int *year, int *month, int *day, int *hour, int *min, int *sec, int *fsec) const;

    ostring GetTimeZone() const;
    void GetTimeZoneOffset(int *hour, int *min) const;

    void AddInterval(const Interval& other);
    void SubstractInterval(const Interval& other);

    void Substract(const Timestamp &other, Interval &result);

    void SysTimestamp();

    void FromString(ostring data, ostring format = OCI_STRING_FORMAT_DATE);
    ostring ToString(ostring format = OCI_STRING_FORMAT_DATE, int precision = 0) const;

    operator ostring() const;

private:

    Timestamp(OCI_Timestamp *pTimestamp, Handle *parent = 0);
};

/**
 * @class Clob
 *
 * @brief
 * Object identififying the SQL datatype CLOB.
 *
 */
class Clob : public HandleHolder<OCI_Lob *>
{
    friend class Statement;
    friend class Resultset;
    friend class BindArray;
    friend class Object;
    friend class Collection;

public:

    Clob(const Connection &connection);

    ostring Read(unsigned int size);
    unsigned int Write(ostring content);
    unsigned int Append(ostring content);
    bool Seek(unsigned int seekMode, big_uint offset);

    void Append(const Clob &other);
    void Assign(const Clob &other);
    bool Equals(const Clob &other) const;

    big_uint GetOffset() const;
    big_uint GetLength() const;
    big_uint GetMaxSize() const;
    big_uint GetChunkSize() const;

    void Truncate(big_uint size);
    big_uint Erase(big_uint offset, big_uint size);
    void Copy(Clob &dest, big_uint offset, big_uint offsetDest, big_uint size) const;

    bool IsTemporary() const;

    void Open(unsigned int mode);
    void Flush();
    void Close();

    void EnableBuffering(bool value);

    operator ostring() const;
	Clob& operator = (const Clob& other);
	Clob& operator + (const Clob& other);
	bool operator == (const Clob& other) const;
	bool operator != (const Clob& other) const;			

private:

    Clob(OCI_Lob *pLob, Handle *parent = 0);
};

/**
 * @class Blob
 *
 * @brief
 * Object identififying the SQL datatype BLOB.
 *
 */
class Blob : public HandleHolder<OCI_Lob *>
{
    friend class Statement;
    friend class Resultset;
    friend class BindArray;
    friend class Object;
    friend class Collection;

public:

    Blob(const Connection &connection);

    unsigned int Read(void *buffer, unsigned int size);
    unsigned int Write(void *buffer, unsigned int size);
    unsigned int Append(void *buffer, unsigned int size);
    bool Seek(unsigned int seekMode, big_uint offset);

    void Append(const Blob &other);
    void Assign(const Blob &other);
    bool Equals(const Blob &other) const;

    big_uint GetOffset() const;
    big_uint GetLength() const;
    big_uint GetMaxSize() const;
    big_uint GetChunkSize() const;

    void Truncate(big_uint size);
    big_uint Erase(big_uint offset, big_uint size);
    void Copy(Blob &dest, big_uint offset, big_uint offsetDest, big_uint size) const;

    bool IsTemporary() const;

    void Open(unsigned int mode);
    void Flush();
    void Close();

    void EnableBuffering(bool value);

	Blob& operator = (const Blob& other);
	Blob& operator + (const Blob& other);
	bool operator == (const Blob& other) const;
	bool operator != (const Blob& other) const;	

private:

    Blob(OCI_Lob *pLob, Handle *parent = 0);
};

/**
 * @class File
 *
 * @brief
 * Object identififying the SQL datatype BFILE.
 *
 */
class File : public HandleHolder<OCI_File *>
{
    friend class Statement;
    friend class Resultset;
    friend class BindArray;
    friend class Object;
    friend class Collection;

public:

    File(const Connection &connection);
    File(const Connection &connection, ostring directory, ostring name);

    unsigned int Read(void *buffer, unsigned int size);
    bool Seek(unsigned int seekMode, big_uint offset);

    void Assign(const File &other);
    bool Equals(const File &other) const;

    bool Exists() const;

    big_uint GetOffset() const;
    big_uint GetSize() const;

    void SetInfos(ostring directory, ostring name);

    ostring GetName() const;
    ostring GetDirectory() const;

    void Open();
    void Close();
    bool IsOpened() const;

private:

    File(OCI_File *pFile, Handle *parent = 0);
};

/**
 * @class TypeInfo
 *
 * @brief
 *vsdgfdgfdhgdfhgfdgd
 */
class TypeInfo : public HandleHolder<OCI_TypeInfo *>
{
    friend class Object;
    friend class Reference;
    friend class Collection;
    friend class Column;
public:

    TypeInfo(const Connection &connection, ostring name, unsigned int type);

    unsigned int GetType() const;
    ostring GetName() const;
    Connection GetConnection() const;

    unsigned int GetColumnCount() const;
    Column GetColumn(unsigned int index) const;

private:

    TypeInfo(OCI_TypeInfo *pTypeInfo);
};

/**
 * @class Object
 *
 * @brief
 * Object identififying the SQL datatype OBJECT.
 *
 */
class Object : public HandleHolder<OCI_Object *>
{
    friend class Statement;
    friend class Resultset;
    friend class BindArray;
    friend class Reference;
    friend class Collection;
    friend class Message;

public:

    Object(const TypeInfo &typeInfo);

    void Assign(const Object& other);

    bool IsAttributeNull(ostring name) const;
    void SetAttributeNull(ostring name);

    TypeInfo GetTypeInfo() const;
    Reference GetReference() const;

    template<class TDataType>
    TDataType Get(ostring name) const;

    template<class TDataType>
    void Set(ostring name, const TDataType &value);

    template<class TDataType>
    void Get(ostring name, TDataType value, unsigned int &size) const;

    template<class TDataType>
    void Set(ostring name, const TDataType &value, unsigned int size);

    ostring ToString() const;
    operator ostring() const;

private:

    Object(OCI_Object *pObject, Handle *parent = 0);
};

/**
 * @class Reference
 *
 * @brief
 * Object identififying the SQL datatype REF.
 *
 */
class Reference : public HandleHolder<OCI_Ref *>
{
    friend class Statement;
    friend class Resultset;
    friend class BindArray;
    friend class Object;
    friend class Collection;

public:

    Reference(const TypeInfo &typeInfo);

    TypeInfo GetTypeInfo() const;
    Object GetObject() const;

    void Assign(const Reference& other);

    bool IsReferenceNull() const;
    void SetReferenceNull();

    ostring ToString() const;
    operator ostring() const;

private:

    Reference(OCI_Ref *pRef, Handle *parent = 0);
};

/**
 * @class Collection
 *
 * @brief
 * Object identififying the SQL datatype VARRAY and NESTED TABLE.
 *
 */
class Collection : public HandleHolder<OCI_Coll *>
{
    friend class Statement;
    friend class Resultset;
    friend class BindArray;
    friend class Object;
    friend class CollectionIterator;
public:

    Collection(const TypeInfo &typeInfo);

    void Assign(const Collection& other);

    unsigned int GetType() const;
    unsigned int GetMax() const;
    unsigned int GetSize() const;
    unsigned int GetCount() const;

    void Truncate(unsigned int size);
    void Clear();

    bool IsElementNull(unsigned int index) const;
    void SetElementNull(unsigned int index);

    bool Delete(unsigned int index) const;

    template <class TDataType>
    TDataType Get(unsigned int index) const;

    template <class TDataType>
    void Set(unsigned int index, const TDataType &data);

    template<class TDataType>
    void Get(unsigned int index, TDataType value, unsigned int &size) const;

    template<class TDataType>
    void Set(unsigned int index, const TDataType value, unsigned int size);

    template <class TDataType>
    void Append(const TDataType &data);

    template <class TDataType>
    void Append(const TDataType &data, unsigned int size);

    TypeInfo GetTypeInfo() const;

    ostring ToString() const;
    operator ostring() const;

private:

    template <class TDataType>
    static TDataType GetElem(OCI_Elem *elem, Handle *parent);

    template <class TDataType>
    static void SetElem(OCI_Elem *elem, const TDataType &value);

    template <class TDataType>
    static void GetElem(OCI_Elem *elem,  TDataType value, unsigned int &size);

    template <class TDataType>
    static void SetElem(OCI_Elem *elem, const TDataType value,  unsigned int size);

    Collection(OCI_Coll *pColl, Handle *parent = 0);
};


/**
 * @class CollectionIterator
 *
 * @brief
 *
 */
class CollectionIterator : public HandleHolder<OCI_Iter *>
{
public:

    CollectionIterator(const Collection &collection);

    template <class TDataType>
    TDataType Get() const;

    template <class TDataType>
    void Set(TDataType &value);

    bool Next();
    bool Prev();

    bool IsElementNull() const;
    void SetElementNull();

    bool operator ++ (int);
    bool operator -- (int value);
};

/**
 * @class CLong
 *
 * @brief
 * Object identififying the SQL datatype LONG.
 *
 */
class CLong : public HandleHolder<OCI_Long *>
{
    friend class Statement;
    friend class Resultset;
    friend class BindArray;

public:

    CLong(const Statement &statement);

    ostring Read(unsigned int size);
    unsigned int Write(ostring content);

    unsigned int GetSize() const;
    ostring GetContent() const;

private:

    CLong(OCI_Long *pLong, Handle *parent = 0);
};

/**
 * @class CLong
 *
 * @brief
 * Object identififying the SQL datatype LONG RAW
 *
 */
class BLong : public HandleHolder<OCI_Long *>
{
    friend class Statement;
    friend class Resultset;
    friend class BindArray;

public:

    BLong(const Statement &statement);

    unsigned int Read(void *buffer, unsigned int size);
    unsigned int Write(void *buffer, unsigned int size);

    unsigned int GetSize() const;
    void * GetContent() const;

private:

    BLong(OCI_Long *pLong, Handle *parent = 0);
};

/**
 * @class BindInfo
 *
 * @brief
 *
 */
class BindInfo : public HandleHolder<OCI_Bind *>
{
    friend class Statement;

public:

    ostring GetName() const;
    unsigned int GetType() const;
    unsigned int GetSubType() const;
    unsigned int GetElemcount() const;

    Statement GetStatement() const;

    void SetNull(bool value);
    void SetNull(bool value, unsigned int pos);

    bool IsNull() const;
    bool IsNull(unsigned int pos) const;

    void SetCharsetForm(unsigned int value);

    void SetDirection(unsigned int value);
    unsigned int GetDirection() const;

private:

    BindInfo(OCI_Bind *pBind, Handle *parent);
};

/**
 * @class Statement
 *
 * @brief
 * Object used for executing SQL or PL/SQL statement and returning the produced results.
 * 
 */
class Statement : public HandleHolder<OCI_Statement *>
{
    friend class Exception;
    friend class Resultset;
    friend class CLong;
    friend class BLong;
    friend class BindInfo;

public:

    Statement(const Connection &connection);
    ~Statement();

    Connection GetConnection() const;

    void Describe(ostring sql);
    void Parse(ostring sql);
    void Prepare(ostring sql);
    void Execute();
    void Execute(ostring sql);

    unsigned int GetAffectedRows() const;

    ostring GetSql()  const;

    Resultset GetResultset();
    Resultset GetNextResultset();

    void SetBindArraySize(unsigned int size);
    unsigned int GetBindArraySize() const;

    void AllowRebinding(bool value);

    unsigned int GetBindCount() const;

    BindInfo GetBind(unsigned int index) const;
    BindInfo GetBind(ostring name) const;

    template <class TDataType>
    void Bind(ostring name, TDataType &value, unsigned int mode);

    template <class TDataType, class TExtraInfo>
    void Bind(ostring name, TDataType &value, TExtraInfo extraInfo, unsigned int mode);

    template <class TDataType>
    void Bind(ostring name, std::vector<TDataType> &values, unsigned int mode);

    template <class TDataType, class TExtraInfo>
    void Bind(ostring name, std::vector<TDataType> &values, TExtraInfo extraInfo, unsigned int mode);

    template <class TDataType, class TExtraInfo>
    void Bind(ostring name, std::vector<TDataType> &values, TExtraInfo &extraInfo, unsigned int mode);

    template <class TDataType>
    void Register(ostring name);

    template <class TDataType, class TExtraInfo>
    void Register(ostring name, TExtraInfo extraInfo);

    template <class TDataType, class TExtraInfo>
    void Register(ostring name, TExtraInfo &extraInfo);

    unsigned int GetStatementType() const;

    unsigned int GetSqlErrorPos() const;

    void SetFetchMode(unsigned int value);
    unsigned int GetFetchMode() const;

    void SetBindMode(unsigned int value);
    unsigned int GetBindMode() const;

    void SetFetchSize(unsigned int value);
    unsigned int GetFetchSize() const;

    void SetPrefetchSize(unsigned int value);
    unsigned int GetPrefetchSize() const;

    void SetPrefetchMemory(unsigned int value);
    unsigned int GetPrefetchMemory() const;

    void SetLongMaxSize(unsigned int value);
    unsigned int GetLongMaxSize() const;

    void SetLongMode(unsigned int value);
    unsigned int GetLongMode() const;

    unsigned int GetSQLCommand() const;
    ostring GetSQLVerb() const;

    void GetBatchErrors(std::vector<Exception> &exceptions);

private:

    Statement(OCI_Statement *stmt, Handle *parent = 0);

    BindsHolder *GetBindsHolder(bool allocate);

    void ReleaseResultsets();

    void SetLastBindMode(unsigned int mode);

    void SetInData();
    void SetOutData();
    void ClearBinds();

    template <typename TBindMethod, class TDataType>
    void Bind (TBindMethod &method, ostring name, TDataType& value, unsigned int mode);

    template <typename TBindMethod, class TObjectType, class TDataType>
    void Bind (TBindMethod &method, ostring name, TObjectType &value, BindValue<TDataType> datatype, unsigned int mode);

    template <typename TBindMethod, class TObjectType, class TDataType>
    void Bind (TBindMethod &method, ostring name, std::vector<TObjectType> &values, BindValue<TDataType> datatype, unsigned int mode);

    template <typename TBindMethod, class TObjectType, class TDataType, class TElemType>
    void Bind (TBindMethod &method, ostring name, std::vector<TObjectType> &values, BindValue<TDataType> datatype, unsigned int mode, TElemType type);
};

/**
 * @class Resultset
 *
 * @brief
 * Database result set
 */
class Resultset : public HandleHolder<OCI_Resultset *>
{
    friend class Statement;
public:

    template<class TDataType>
    TDataType Get(unsigned int index) const;

    template<class TDataType>
    TDataType Get(ostring name) const;

    template<class TDataType>
    void Get(unsigned int index, TDataType value, unsigned int &size) const;

    template<class TDataType>
    void Get(ostring name, TDataType value, unsigned int &size) const;

    bool Next();
    bool Prev();
    bool First();
    bool Last();
    bool Seek(unsigned int mode, int offset);

    unsigned int GetCount() const;
    unsigned int GetCurrentRow() const;

    unsigned int GetColumnCount() const;
    Column GetColumn(unsigned int index) const;
    Column GetColumn(ostring name) const;

    bool IsColumnNull(unsigned int index) const;
    bool IsColumnNull(ostring name) const;

    Statement GetStatement() const;

    bool operator ++ (int);
    bool operator -- (int value);

    bool operator += (int);
    bool operator -= (int value);

private:

   Resultset(OCI_Resultset *resultset, Handle *parent);
};

/**
 * @class Column
 *
 * @brief
 *
 */
class Column : public HandleHolder<OCI_Column *>
{
    friend class TypeInfo;
    friend class Resultset;

public:

    ostring GetName() const;
    ostring GetSQLType() const;
    ostring GetFullSQLType() const;

    unsigned int GetType() const;
    unsigned int GetSubType() const;
    unsigned int GetCharsetForm() const;
    unsigned int GetSize() const;

    int GetScale() const;
    int GetPrecision() const;
    int GetFractionalPrecision() const;
    int GetLeadingPrecision() const;

    bool GetNullable() const;
    bool GetCharUsed() const;

    TypeInfo GetTypeInfo() const;

private:

    Column(OCI_Column *pColumn, Handle *parent);
};

/**
 * @class Subscription
 *
 * @brief
 *
 */
class Subscription : public HandleHolder<OCI_Subscription *>
{
    friend class Event;

public:

    Subscription();

    void Register(const Connection &con, ostring name, unsigned int type, NotifyHandlerProc handler, unsigned int port = 0, unsigned int timeout = 0);
    void Unregister();

    void Watch(ostring sql);

    ostring GetName() const;
    unsigned int GetTimeout() const;
    unsigned int GetPort() const;

    Connection GetConnection() const;

private:

    Subscription(OCI_Subscription *pSubcription);
};

/**
 * @class Event
 *
 * @brief
 *dfsdgfsdgsdgsdgsdgsd
 */
class Event : public HandleHolder<OCI_Event *>
{
    friend class Subscription;
    friend class Environment;

public:

    unsigned int GetType() const;
    unsigned int GetOperation() const;
    ostring GetDatabaseName() const;
    ostring GetObjectName() const;
    ostring GetRowID() const;
    Subscription GetSubscription() const;

private:

    Event(OCI_Event *pEvent);
};


/**
 * @class Agent
 *
 * @brief
 *
 */
class Agent : public HandleHolder<OCI_Agent *>
{
    friend class Message;
    friend class Dequeue;

public:

    Agent(const Connection &connection, ostring name = OTEXT(""), ostring address = OTEXT(""));

    ostring GetName() const;
    void SetName(ostring value);

    ostring GetAddress() const;
    void SetAddress(ostring value);

private:

    Agent(OCI_Agent *pAgent, Handle *parent);
};

/**
 * @class Message
 *
 * @brief
 *
 */
class Message : public HandleHolder<OCI_Msg *>
{
    friend class Dequeue;

public:

    Message(const TypeInfo &typeInfo);

    void Reset();

    Object Get();
    void Set(const Object &value);

    void Get(BufferPointer value, unsigned int &size);
    void Set(const BufferPointer &value, unsigned int size);

    Date GetEnqueueTime() const;
    int GetAttemptCount() const;
    unsigned int GetState() const;
    void GetID(BufferPointer value, unsigned int &size) const;

    int GetExpiration() const;
    void SetExpiration(int value);

    int GetEnqueueDelay() const;
    void SetEnqueueDelay(int value);

    int GetPriority() const;
    void SetPriority(int value);

    void GetOriginalID(BufferPointer value, unsigned int &size) const;
    void SetOriginalID(const BufferPointer &value, unsigned int size);

    ostring GetCorrelation() const;
    void SetCorrelation(ostring value);

    ostring GetExceptionQueue() const;
    void SetExceptionQueue(ostring value);

    Agent GetSender() const;;
    void SetSender(const Agent &agent);

    void SetConsumers(std::vector<Agent> &agents);

private:

    Message(OCI_Msg *pMessage, Handle *parent);
};

/**
 * @class Enqueue
 *
 * @brief
 *
 */
class Enqueue : public HandleHolder<OCI_Enqueue *>
{
public:

    Enqueue(const TypeInfo &typeInfo, ostring queueName);

    void Put(const Message &message);

    unsigned int GetVisibility() const;
    void SetVisibility(unsigned int value);

    unsigned int GetSequenceDeviation() const;
    void SetSequenceDeviation(unsigned int value);

    void GetRelativeMsgID(BufferPointer value, unsigned int &size) const;
    void SetRelativeMsgID(const BufferPointer &value, unsigned int size);
};

/**
 * @class Dequeue
 *
 * @brief
 *
 */
class Dequeue : public HandleHolder<OCI_Dequeue *>
{
    friend class Environment;

public:

    Dequeue(const TypeInfo &typeInfo, ostring queueName);

    Message Get();

    Agent Listen(int timeout);

    ostring GetConsumer() const;
    void SetConsumer(ostring value);

    ostring GetCorrelation() const;
    void SetCorrelation(ostring value);

    void GetRelativeMsgID(BufferPointer value, unsigned int &size) const;
    void SetRelativeMsgID(const BufferPointer &value, unsigned int size);

    unsigned int GetVisibility() const;
    void SetVisibility(unsigned int value);

    unsigned int GetMode() const;
    void SetMode(unsigned int value);

    unsigned int GetNavigation() const;
    void SetNavigation(unsigned int value);

    int GetWaitTime() const;
    void SetWaitTime(int value);

    void SetAgents(std::vector<Agent> &agents);

    void Subscribe(unsigned int port, unsigned int timeout, NotifyAQHandlerProc handler);
    void Unsubscribe();

private:

    Dequeue(OCI_Dequeue *pDequeue);
};

/**
 * @class Queue
 *
 * @brief
 *
 */
class Queue
{
public:

    static void Create(const Connection &connection, ostring queue, ostring table, unsigned int queueType = OCI_AQT_NORMAL, unsigned int maxRetries = 0,
                       unsigned int retryDelay = 0, unsigned int retentionTime = 0, bool dependencyTracking = false, ostring comment = OTEXT(""));
    static void Alter (const Connection &connection, ostring queue, unsigned int maxRetries= 0, unsigned int retryDelay= 0, unsigned int retentionTime= 0, ostring comment = OTEXT(""));
    static void Drop  (const Connection &connection, ostring queue);
    static void Start (const Connection &connection, ostring queue, bool startEnqueue = true, bool startDequeue = true);
    static void Stop  (const Connection &connection, ostring queue, bool stopEnqueue = true, bool stopDequeue = true, bool wait = true);
};

/**
 * @class QueueTable
 *
 * @brief
 * Static class using for manipulating AQ Tables
 *
 */
class QueueTable
{
public:

    static void Create (const Connection &connection, ostring table, ostring payloadType, bool multipleConsumers, ostring storageClause = OTEXT(""), ostring sortList = OTEXT(""),
                        unsigned int messageGrouping = OCI_AGM_NONE, ostring comment = OTEXT(""), unsigned int primaryInstance = 0, unsigned int secondaryInstance = 0, ostring compatible = OTEXT(""));
    static void Alter  (const Connection &connection, ostring table, ostring comment, unsigned int primaryInstance = 0, unsigned int secondaryInstance = 0);
    static void Drop   (const Connection &connection, ostring table, bool force = true);
    static void Purge  (const Connection &connection, ostring table,  unsigned int deliveryMode, ostring purgeCondition = OTEXT(""), bool block = true);
    static void Migrate(const Connection &connection, ostring table, ostring compatible = OTEXT(""));
};

/**
 * @class DirectPath
 *
 * @brief
 *
 */
class DirectPath : public HandleHolder<OCI_DirPath *>
{
public:

    DirectPath(const TypeInfo &typeInfo, unsigned int nbCols, unsigned int  nbRows, ostring partition = OTEXT(""));

    void SetColumn(unsigned int colIndex, ostring name, unsigned int maxSize,  ostring format = OTEXT(""));
    void SetEntry(unsigned int rowIndex, unsigned int colIndex,  const ostring &value,  bool complete = true);
    void SetEntry(unsigned int rowIndex, unsigned int colIndex,  const BufferPointer &value, unsigned int size,  bool complete = true);

    void Reset();
    void Prepare();
    unsigned int Convert();
    unsigned int Load();
    void Finish();
    void Abort();
    void Save();
    void FlushRow();

    void SetCurrentRows(unsigned int nbRows);
    unsigned int GetCurrentRows() const;

    unsigned int GetMaxRows() const;
    unsigned int GetRowCount() const;
    unsigned int GetAffectedRows() const;

    void SetDateFormat(ostring format);

    void SetParallel(bool value);
    void SetNoLog(bool value);
    void SetCacheSize(unsigned int value);
    void SetBufferSize(unsigned int value);

    void SetConvertMode(unsigned int value);

    unsigned int GetErrorColumn();
    unsigned int GetErrorRow();
};

 /**
 *
 * @}
 */


#include "ocilib_impl.hpp"

}

#endif

