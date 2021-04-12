.include "__includes.s"
.set INJECTADDR, 0x80341DE8

#inject(0x80341DE8)
.set ISPYIIASMH, TRUE
.if ISPYIIASMH == FALSE
.macro __set register value
.if \value > 0x7FFF || \value < -0x7FFF
.if ((\value >> 16) != 0) && ((\value & 0xFFFF) == 0)
lis \register, \value@h
.else
lis \register, \value@h
ori \register, \register, \value & 0xFFFF
.endif
.else
li \register, \value
.endif
.endm
.macro __call register address
.if \address == NONE
mtctr \register
.else
__set \register \address
mtctr \register
.endif
bctrl
.endm
.endif
# SMS NTSC-U Kuribo Loader #
#     Credits: Riidefi     #
.set DVDOpen, 0x8034B914
.set DVDReadPrio, 0x8034BD74
.set DVDClose, 0x8034B9DC
#
.set OSReport, 0x80344644
# As this may be a cheat code, we use absolute branching with __call
.global loadCMX
loadCMD:
.set back_chain, -0x50
.set istate, -0x4c
.set dvd_handle, -0x48
.set r30_save, -8
.set r31_save, -4
.set sender_lr,  4
stwu      r1, back_chain(r1)
mflr      r0
stw       r0, 0x50+sender_lr(r1)
addi      r4, r1, 0x50+dvd_handle
stw       r31, 0x50+r31_save(r1)
stw       r30, 0x50+r30_save(r1)
# Disable interrupts, saving state
mfmsr     r3
rlwinm    r12, r3, 0,17,15
mtmsr     r12
extrwi    r3, r3, 1,16
stw       r3, 0x50+istate(r1)
# Try to open the file
bl        cometBinStrEnd
.string   "Kuribo!/System/KuriboKernel.bin"
.align 4
cometBinStrEnd:
mflr r3
__call    r12, DVDOpen
cmpwi     r3, 0
beq       fail
# Load filesize, round 32B
lwz       r3, 0x50-0x14(r1)
addi      r0, r3, 0x1F
clrrwi    r31, r0, 5
lis r4, 0x808e
ori r4, r4, 0xe920
mr r30, r4
# Read the file
# mr        r4, r30
mr        r5, r31
addi      r3, r1, 0x50+dvd_handle
li        r6, 0
li        r7, 2
__call    r12, DVDReadPrio
# Close the file
addi      r3, r1, 0x50+dvd_handle
__call    r12, DVDClose
# Call the prologue
#    lwz       r4, 0x04(r30)
#    mr        r3, r30
#    mtctr     r4
mtctr r30
bctrl
bl        succStringEnd
.string   "===\nKuribo Loader v1: Loaded\n===\n"
.align 4
succStringEnd:
mflr r3
crclr     4*cr1+eq
__call    r12, OSReport
b         return
fail:
bl        failStrEnd
.string   "===\nKuribo Loader v1: Failed to load. Make sure you have copied the folder onto your ISO!\n===\n"
.align 4
failStrEnd:
mflr r3
crclr     4*cr1+eq
__call    r12, OSReport
return:
# Restore interrupts OSRestoreInterrupts
lwz       r3, 0x50+istate(r1)
cmpwi     r3, 0
mfmsr     r4
beq       unset
ori       r5, r4, 0x8000
b         end
unset:
rlwinm    r5, r4, 0,17,15
end:
mtmsr     r5
extrwi    r3, r4, 1,16
lwz       r0, 0x50+sender_lr(r1)
lwz       r31, 0x50+r31_save(r1)
lwz       r30, 0x50+r30_save(r1)
mtlr      r0
addi      r1, r1, 0x50
li r0, 0
