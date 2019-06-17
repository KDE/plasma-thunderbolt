#=============================================================================
# Copyright 2019 Daniel Vrátil <dvratil@kde.org>
# Copyright 2019 Harald Sitter <sitter@kde.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#=============================================================================

# On Debian systems the daemon can be in side various lib directories...
set(boltd_PATHS
    /usr/lib/bolt/ # 0.5+ on Debian and Ubuntu
)
if(CMAKE_LIBRARY_ARCHITECTURE)
    list(APPEND boltd_PATHS
        # 0.5 on Ubuntu
        /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/
    )
endif()

find_program(boltd_EXECUTABLE
    NAMES boltd
    PATHS ${boltd_PATHS}
    PATH_SUFFIXES bin libexec
)
find_package_handle_standard_args(bolt
    FOUND_VAR
        bolt_FOUND
    REQUIRED_VARS
        boltd_EXECUTABLE
)
mark_as_advanced(boltd_EXECUTABLE)
