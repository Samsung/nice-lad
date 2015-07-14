# nice-lad

## Introduction
Narcissistic, Incredible, Completely Exceptional Logger of Access Denials

### Project goals
Nice-lad is a tool to collect and aggregate logs of access denials in system.
The source of data are audit messages from DAC, Smack, Cynara and netfilter.

The purpose of nice-lad is to collect and normalize the selected audit logs
and make them readable by unprivileged user. This might be helpful during
debugging applications accessing restricted resources.

Nice-lad works as an [audispd][1] plugin.

[1]: http://linux.die.net/man/8/audispd "audispd"

### Project history
Nice-lad was first introduced in July 2015.

### Contact information
| Name            | E-mail                  | Function              |
|-----------------|-------------------------|-----------------------|
| Aleksander Zdyb | [a.zdyb@samsung.com][2] | Developer, Maintainer |

[2]: mailto:a.zdyb@samsung.com "Aleksander Zdyb"

### Sources
The equivalent places, nice-lad can be obtained from:

* [Tizen](https://review.tizen.org/gerrit/gitweb?p=platform/core/security/nice-lad.git)
* [GitHub](https://github.com/Samsung/nice-lad)


## Features
Nice-lad, as an audisp plugin, is fed with audit events. It parses and filters
them to obtain and aggregate information useful in context of logging of access
denials.

At the moment, the supported subsystems are:

* DAC denials on given groups,
* Smack denials,
* [Cynara][3] denials,
* Netfilter denials (supported by [Nether][4]).

[3]: https://github.com/Samsung/cynara "Cynara"
[4]: https://github.com/Samsung/nether "Nether"

### Implanted standards
Nice-lad uses:

* libauparse to parse audit events,
* [Security Manager][5] (where available) to obtain resource groups to monitor,
* journald (where available) or syslog to put aggregated logs.

[5]: https://github.com/Samsung/security-manager "Security Manager"


## Running the project
The package consists of following files (note, the exact paths are
system-dependent):

* /etc/audisp/plugins.d/nice_lad.conf,
* /usr/sbin/nice-lad.

Provided, the above config file is present in audisp plugins directory, nice-lad
is automagically activated, when auditing service is run. In order to disable
nice-lad, while keeping audit running, one need to edit the config to contain
"active = no".

### Reading the logs
Nice-lad will log access denials to journald (if available) or syslog with
informational level. Below, are some examples:

```
Jul 10 10:11:04 HOSTNAME nice-lad: ACCESS DENIED ON SYSCALL syscall=open filename=/tmp/test exit=-13(Permission denied) gid=unknown(1234) object=test subject=_
Jul 10 10:11:09 HOSTNAME nice-lad: ACCESS DENIED ON SMACK object="test" subject="_" access=r
Jul 10 10:11:26 HOSTNAME nice-lad: ACCESS DENIED ON CYNARA client="test_client" user="test_user" privilege="http://tizen.org/privilege/account.read"
Jul 10 10:11:51 HOSTNAME nice-lad: ACCESS DENIED ON NETFILTER obj=User outif=eth0 proto=tcp saddr=10.0.2.16 sport=54460 daddr=198.145.20.7 dport=443
```

### Testing
Nice-lad comes with a set of unit tests written in [gmock][6]. By adding new
features or fixing bugs, please add or update tests.

[6]: http://gmock.org/ "Gmock"
