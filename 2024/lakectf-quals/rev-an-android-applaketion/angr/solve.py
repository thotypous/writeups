#!/usr/bin/env python
import angr
import claripy

p = angr.Project("ohgreat", auto_load_libs=True)
arg1 = claripy.BVS('arg1', 63*8)
#initial_state = p.factory.entry_state(args=["ohgreat", claripy.Concat(arg1, claripy.BVV(b'\x00'))])
initial_state = p.factory.entry_state(stdin=claripy.Concat(arg1, claripy.BVV(b'\n')))
for b in arg1.chop(8):
    initial_state.add_constraints(b >= 0x21, b <= 0x7e)
sm = p.factory.simulation_manager(initial_state)
sm.explore(
    find=(0x40197e, ),
    avoid=(0x4019fc, )
)
found = sm.found[0]
print(bytes.fromhex(hex(found.solver.eval(arg1))[2:]))
