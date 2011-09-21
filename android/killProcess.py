# Kill a running Android JDWP (debuggable) process without Eclipse
#
import sys
import socket
import struct
import uuid
import random
import subprocess

pid = sys.argv[1]

forward_port = 51112
print 'Forwarding PID %s to Port %d ...' % (pid, forward_port)
# adb -s <device id> forward tcp:<port> jdwp:<pid>
subprocess.Popen(['adb', 'forward', 'tcp:%d' % forward_port, 'jdwp:%s' % pid]).wait()

print 'Connecting via JDWP...'
jdwp = socket.create_connection(('127.0.0.1', forward_port))
jdwp.settimeout(15.0)
print 'Sending Handshake...'
sent = jdwp.send('JDWP-Handshake')

handshake = jdwp.recv(14)

print 'Rcvd Handshake (%s)' % handshake
if handshake != 'JDWP-Handshake':
    print '-> Error: Incorrect handshake. Check adb jdwp to make sure the PID is listed. aborting.'
    jdwp.close()
    sys.exit(1)

DDMS_CMD = 0x01
DDMS_CMD_SET = 0xc7
# just a random 32 bit integer should be good enough
packet_id = uuid.uuid4().time_low
packet_len = 23

# the string EXIT as an integer
EXIT = 1163413844
EXIT_LEN = 4
exit_code = 1

print 'Sending Exit Command...'
packet = struct.pack('!2I3B3I', packet_len, packet_id, 0, DDMS_CMD_SET, DDMS_CMD, EXIT, EXIT_LEN, exit_code)
jdwp.send(packet)

print 'Success!'
jdwp.close()
