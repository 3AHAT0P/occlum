use crate::error::*;
use crate::time::timeval_t;
use std::convert::TryFrom;

use crate::vm::{ChunkType, VMArea, VMPerms, VMRange, PAGE_SIZE};

#[repr(C)]
#[derive(Default)]
pub struct Rusage {
    ru_utime: timeval_t,
    ru_stime: timeval_t,

    // For x86-64, `long int` is 64-bit long. Just use i64 here
    ru_maxrss: i64,   // maximum resident set size
    ru_ixrss: i64,    // integral shared memory size
    ru_idrss: i64,    // integral unshared data size
    ru_isrss: i64,    // integral unshared stack size
    ru_minflt: i64,   // page reclaims (soft page faults)
    ru_majflt: i64,   // page faults (hard page faults)
    ru_nswap: i64,    // swaps
    ru_inblock: i64,  // block input operations
    ru_oublock: i64,  // block output operations
    ru_msgsnd: i64,   // IPC messages sent
    ru_msgrcv: i64,   // IPC messages received
    ru_nsignals: i64, // signals received
    ru_nvcsw: i64,    // voluntary context switches
    ru_nivcsw: i64,   // involuntary context switches
}

#[derive(Debug)]
#[repr(i32)]
#[allow(non_camel_case_types)]
pub enum RusageWho {
    RUSAGE_SELF = 0,
    RUSAGE_CHILDREN = -1,
    RUSAGE_THREAD = 1,
}

impl TryFrom<i32> for RusageWho {
    type Error = Error;

    fn try_from(value: i32) -> Result<Self> {
        match value {
            0 => Ok(RusageWho::RUSAGE_SELF),
            -1 => Ok(RusageWho::RUSAGE_CHILDREN),
            1 => Ok(RusageWho::RUSAGE_THREAD),
            _ => return_errno!(EINVAL, "invalid rusage who"),
        }
    }
}

pub fn do_getrusage(who: RusageWho, rusage: &mut Rusage) -> Result<()> {
    debug!("getrusage who: {:?}", who);
    let process = current!().process();
    let main_thread = process.main_thread().unwrap();
    let process_vm = main_thread.vm();// Measured in pages
    let virtual_mem_usage = process_vm.get_in_use_size() / 1024; // in kibibytes
    let mut zero_rusage = Rusage::default();
    zero_rusage.ru_maxrss = virtual_mem_usage as i64;
    zero_rusage.ru_utime = process.real_time();

    core::mem::swap(rusage, &mut zero_rusage);
    Ok(())
}