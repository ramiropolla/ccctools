/*
 * Copyright (c) 2003-2007 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * "Portions Copyright (c) 2003 Apple Computer, Inc.  All Rights
 * Reserved.  This file contains Original Code and/or Modifications of
 * Original Code as defined in and that are subject to the Apple Public
 * Source License Version 1.0 (the 'License').  You may not use this file
 * except in compliance with the License.  Please obtain a copy of the
 * License at http://www.apple.com/publicsource and read it before using
 * this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License."
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef __NOTIFICATION_H__
#define __NOTIFICATION_H__

#include <sys/cdefs.h>
#include <stdint.h>
#include <mach/message.h>
#include <Availability.h>
#ifdef __BLOCKS__
#include <dispatch/dispatch.h>
#endif /* __BLOCKS__ */

/*! @header
 * These routines allow processes to exchange stateless notification events.
 * Processes post notifications to a single system-wide notification server,
 * which then distributes notifications to client processes that have
 * registered to receive those notifications, including processes run by
 * other users.
 *
 * Notifications are associated with names in a namespace shared by all
 * clients of the system.  Clients may post notifications for names, and
 * may monitor names for posted notifications.  Clients may request
 * notification delivery by a number of different methods.
 *
 * Clients desiring to monitor names in the notification system must
 * register with the system, providing a name and other information
 * required for the desired notification delivery method.  Clients are
 * given an integer token representing the registration.
 *
 * Note that the kernel provides limited queues for mach message and file
 * descriptor messages.  It is important to make sure that clients read
 * mach ports and file descriptors frequently to prevent messages from
 * being lost due to resource limitations.  Clients that use signal-based
 * notification should be aware that signals are not delivered to
 * a process while it is running in a signal handler.  This may affect
 * the delivery of signals in close succession.
 *
 * Notifications may be coalesced in some cases.  Multiple events posted
 * for a name in rapid succession may result in a single notification sent
 * to clients registered for notification for that name.  Clients checking
 * for changes using the notify_check() routine cannot determine if
 * more than one event pas been posted since a previous call to 
 * notify_check() for that name.
 *
 * "False positives" may occur in notify_check() when used with a token
 * generated by notify_register_check() due to implementation constraints.
 * This behavior may vary in future releases.  
 *
 * Synchronization between two processes may be achieved using the
 * notify_set_state() and notify_get_state() routines.
 */

/*! @defineblock Status Codes
 * Status codes returned by the API.
 */
#define NOTIFY_STATUS_OK 0
#define NOTIFY_STATUS_INVALID_NAME 1
#define NOTIFY_STATUS_INVALID_TOKEN 2
#define NOTIFY_STATUS_INVALID_PORT 3
#define NOTIFY_STATUS_INVALID_FILE 4
#define NOTIFY_STATUS_INVALID_SIGNAL 5
#define NOTIFY_STATUS_INVALID_REQUEST 6
#define NOTIFY_STATUS_NOT_AUTHORIZED 7
#define NOTIFY_STATUS_FAILED 1000000
/*! @/defineblock */

/*!
 * Flag bits used for registration.
 */
#define NOTIFY_REUSE 0x00000001


__BEGIN_DECLS

/*!
 * Post a notification for a name.
 *
 * This is the only call that is required for a notification producer.
 * Returns status.
 */
uint32_t notify_post(const char *name);


#ifdef __BLOCKS__
typedef void (^notify_handler_t)(int token);

/*!
 * @function   notify_register
 * @abstract   Request notification delivery to a dispatch queue.
 * @discussion When notifications are received by the process, the notify
 *             subsystem will deliver the registered Block to the target
 *             dispatch queue.  Notification blocks are not re-entrant,
 *             and subsequent notification Blocks will not be delivered
 *             for the same registration until the previous Block has
 *             returned.
 * @param name (input) The notification name.
 * @param out_token (output) The registration token.
 * @param queue (input) The dispatch queue to which the Block is submitted.
 *              The dispatch queue is retained by the notify subsystem while
 *              the notification is registered, and will be released when
 *              notification is canceled.
 * @param block (input) The Block to invoke on the dispatch queue in response
 *              to a notification.  The notification token is passed to the
 *              Block as an argument so that the callee can modify the state
 *              of the notification or cancel the registration.
 * @result Returns status.
 */
uint32_t notify_register_dispatch(const char *name, int *out_token, dispatch_queue_t queue, notify_handler_t handler)
__OSX_AVAILABLE_STARTING(__MAC_10_6,__IPHONE_3_2);
#endif /* __BLOCKS__ */

/*!
 * Creates a registration token be used with notify_check(),
 * but no active notifications will be delivered.
 *
 * @param name
 *    (input) notification name
 * @param out_token
 *    (output) registration token
 * @result Returns status.
 */
uint32_t notify_register_check(const char *name, int *out_token);

/*!
 * Request notification delivery by UNIX signal.
 *
 * A client may request signal notification for multiple names.  After a signal
 * is delivered, the notify_check() routine may be called with each notification 
 * token to determine which name (if any) generated the signal notification.
 *
 * @param name (input) notification name
 * @param sig (input) signal number (see signal(3))
 * @param out_token (output) notification token
 * @result Returns status.
 */
uint32_t notify_register_signal(const char *name, int sig, int *out_token);

/*!
 * Request notification by mach message.  
 *
 * Notifications are delivered by an empty message sent to a mach port.
 * By default, a new port is allocated and a pointer to it is returned
 * as the value of "notify_port".  A mach port previously returned by a 
 * call to this routine may be used for notifications if a pointer to that
 * port is passed in to the routine and NOTIFY_REUSE is set in the flags
 * parameter.  The notification service must be able to extract send
 * rights to the port.
 *
 * Note that the kernel limits the size of the message queue for any port.
 * If it is important that notifications should not be lost due to queue
 * overflow, clients should service messages quickly, and be careful about
 * using the same port for notifications for more than one name.
 *
 * A notification message has an empty message body.  The msgh_id field
 * in the mach message header will have the value of the notification
 * token.  If a port is reused for multiple notification registrations,
 * the msgh_id value may be used to determine which name generated
 * the notification.
 *
 * @param name
 *     (input) notification name
 * @param  out_token
 *     (output) notification token
 * @param  notify_port
 *     (input/output) pointer to a mach port
 * @result Returns status.
 */
uint32_t notify_register_mach_port(const char *name, mach_port_t *notify_port, int flags, int *out_token);

/*
 * Request notification by a write to a file descriptor. 
 *
 * Notifications are delivered by a write to a file descriptor.
 * By default, a new file descriptor is created and a pointer to it
 * is returned as the value of "notify_fd".  A file descriptor created
 * by a previous call to this routine may be used for notifications if
 * a pointer to that file descriptor is passed in to the routine and
 * NOTIFY_REUSE is set in the flags parameter. 
 *
 * Note that the kernel limits the buffer space for queued writes on a
 * file descriptor.  If it is important that notifications should not be
 * lost due to queue overflow, clients should service messages quickly,
 * and be careful about using the same file descriptor for notifications
 * for more than one name.
 *
 * Notifications are delivered by an integer value written to the
 * file descriptor.  The value will match the notification token
 * for which the notification was generated.
 *
 * @param name
 *     (input) notification name
 * @param out_token
 *     (output) notification token
 * @param notify_fd
 *     (input/output) pointer to a file descriptor
 * @result Returns status.
 */
uint32_t notify_register_file_descriptor(const char *name, int *notify_fd, int flags, int *out_token);

/*!
 * Check if any notifications have been posted.
 *
 * Output parameter check is set to 0 for false, 1 for true.  Returns status.
 * check is set to true the first time notify_check is called for a token.
 * Subsequent calls set check to true when notifications have been posted for
 * the name associated with the notification token.  This routine is independent
 * of notify_post().  That is, check will be true if an application calls
 * notify_post() for a name and then calls notify_check() for a token associated
 * with that name.
 *
 * @param token
 *     (input)notification token
 * @param check
 *     (output) true/false indication
 * @result Returns status.
 */
uint32_t notify_check(int token, int *check);

/*!
 * Cancel notification and free resources associated with a notification
 * token.  Mach ports and file descriptor associated with a token are released 
 * (deallocated or closed) when all registration tokens associated with 
 * the port or file descriptor have been cancelled.
 *
 * @param token
 *     (input) notification token
 * @result Returns status.
 */
uint32_t notify_cancel(int token);

/*!
 * Set or get a state value associated with a notification token.
 * Each key in the notification namespace has an associated integer value available
 * for use by clients as for application-specific purposes.  A common usage is to 
 * allow two processes or threads to synchronize their activities.  For example, a
 * server process may need send a notification when a resource becomes available.
 * A client process can register for the notification, but when it starts up it will
 * not know whether the resource is available.  The server can set the state value,
 * and the client can check the value at startup time to synchronize with the server.
 *
 * Set the 64-bit integer state value.
 *
 * @param token
 *     (input) notification token
 * @param state64
 *     (input) 64-bit unsigned integer value
 * @result Returns status.
 */
uint32_t notify_set_state(int token, uint64_t state64)
__OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_2_0);

/*!
 * Get the 64-bit integer state value.
 *
 * @param token
 *     (input) notification token
 * @param state64
 *     (output) 64-bit unsigned integer value
 * @result Returns status.
 */
uint32_t notify_get_state(int token, uint64_t *state64)
__OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_2_0);

__END_DECLS

#endif /* __NOTIFICATION_H__ */
