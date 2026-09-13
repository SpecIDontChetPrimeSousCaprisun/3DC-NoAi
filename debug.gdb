# debug.gdb
set auto-load no
set print thread-events off
run
if $_isvoid($_exitcode)
  bt
end
