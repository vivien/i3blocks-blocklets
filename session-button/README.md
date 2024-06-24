

# session-button

Creates a button for logging out or locking the user session.

![](logout.png) ![](lock.png) ![](error.png)


# Dependencies

- The [bash](https://www.gnu.org/software/bash/bash.html) shell or a
compatible one.


# Optional

- [systemd](https://freedesktop.org/wiki/Software/systemd/) or
[elogind](https://github.com/elogind/elogind) for the ``loginctl`` command.

- [Font Awesome](https://fontawesome.com) for the default
label icons.

- [libnotify](https://developer.gnome.org/notification-spec/) for
error notifications via ``notify-send``.


# Usage

Scrolling the mouse wheel or right-clicking selects the action to take, left-
clicking performs it. If the action fails, an error label
( ![](error.png) ) is displayed and if available, ``notify-send`` is used
to send a notification.

The default logout command is ``loginctl kill-session $XDG_SESSION_ID``. It can
be overridden by setting configuration variable ``LOGOUT_CMD``, which is
mandatory if neither ``systemd`` nor ``elogind`` are running.

The default lock command is ``i3lock``. It can be overridden by setting
configuration variable ``LOCK_CMD``, which is mandatory if ``i3`` is not
running.

A pango font fallback must be set in the window manager's bar
configuration ([see the i3blocks FAQ](https://github.com/vivien/i3blocks#faq)).
It should include FontAwesome for the default labels to be displayed correctly.


# Config

```INI
[session-button]
## For a standard installation of i3blocks-contrib, i.e.
##
## make install
## status_command SCRIPT_DIR=<installation directory> i3blocks  # in the window manager's bar config
## command=$SCRIPT_DIR/$BLOCK_NAME  # as a global i3blocks config property
##
## no command property is needed. 
#command=$SCRIPT_DIR/session-button
color=#00a800
background=#000000
align=center
min_width=XX
interval=persist
markup=pango
## This must be defined if neither systemd nor elogind are running:
#LOGOUT_CMD=loginctl kill-session $XDG_SESSION_ID
## This must be defined if i3 is not running (swaylock throws an error atm):
#LOCK_CMD=i3lock
## The default labels need font-awesome. Spaces are allowed,
## ',' ':' '\' '$' '`' are not allowed.
#LOGOUT_LABEL=
#LOCK_LABEL=
#ERROR_LABEL=
```
