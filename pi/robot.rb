#!/usr/bin/env ruby

system("echo #{Process.pid} > /var/log/web.pid")

while true
  sleep 1
end
