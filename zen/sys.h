/*
 * system call groups
 * ------------------
 * - mach
 *   - machctl
 * - sys
 *   - sysctl
 * - dev
 *   - devctl, devconf, devprobe, devinit,
 *   - devopen, devclose, devread, devwrite, devseek, devsetbuf
 * - task
 *   - taskctl, taskexit, taskwait, tasksleep
 *   - thrctl, thrjoin, thrsleep, thrwait, thrsig, threxit
 * - mem
 * memctl, memget, memmap, memconf, memfree
 * - io
 *   - fctl(fd, cmd, arg)
 *   - flock(fd, ofs, len)
 *   - fmap(fd, ofs, len, adr, perm, flg)
 *   - fopen(path, flg)
 *   - fclose(fd)
 *   - fread(fd, buf, len)
 *   - fwrite(fd, buf, len)
 *   - fseek(fd, ofs, orig)
 * - sig
 *   SIGINV             0xff
 *   SIGILL             0x01
 *   SIGMEM             0x02
 *   SIGIO              0x03
 *   SIGSTOP            0x04
 *   SIGCONT            0x05
 *   SIGTERM            0x06
 *   SIGFAIL            0x07
 *   SIGKILL            0x08
 *   SIGMATH            0x09
 *   SIGCOPROC          0x0a
 *   SIGUSR1            0xob
 *   SIGUSR2            0x0c
 *   SIGRT1             0x10
 *   SIGRT16            0x1f
 *
 *   - sigset(sig, func)        set signal disposition
 *     SIG_DFL - ((void *)0)
 *     SIG_IGN - ((void *)1)
 *   - sigsend(sig, proc)       dispatch or raise signal
 *                      x       process ID x
 *     PROC_SELF        0       current process
 *     PROC_CHILDREN    -1      all children of current process
 *                      -x      all children of process x
 *   - sigmask()                fetch signal mask
 *   - sigblk(mask)             add signals to mask
 *   - sigunblk(sig)            clear signals from mask
 *   - sigwait(mask)            wait for a signal in mask
 * - ipc
 *   - ipcctl, ipcmkobj, ipcrm, ipcsend, ipcrecv
 */

