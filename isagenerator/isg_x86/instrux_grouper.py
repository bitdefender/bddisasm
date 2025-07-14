#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from __future__ import annotations
from dataclasses import dataclass
from typing import Union

from isg_x86 import instrux


@dataclass
class GroupComponent:
    """
    Describes a grouping component.

    Attributes
    ----------
    Type: str
        Group name.
    Size: int
        Indicates the size of the lookup table used to index in this grouping component.
    All: bool
        Indicates whether all instruction in the group must be present.
    Filter: bool
        Indicates whether the grouping component is a filter.
    """
    Type    : str
    Size    : int
    All     : bool
    Filter  : bool = False


@dataclass
class InstructionGroup:
    """
    Describes an instruction group. Instructions in the same group are selected based on one 
    decoding component (example: the opcode), which is indexed in the Children array, to select
    the next method of lookup/decoding, until we reach and entry that is a leaf.

    Attributes
    ----------
    Component: GroupComponent
        One of the grouping components in components_legacy or components_ex.
    Children: Union[list[InstructionGroup], instrux.Instruction]
        A list of children instruction groups, or a leaf instruction, if component is None
    """
    Component : GroupComponent
    Children  : Union[list[InstructionGroup], instrux.Instruction]


#
# These are the encoding components used to group instructions. Important things to note:
# - The order here is important! The higher up (the lower the index) for a given component, the more priority it 
#   receives when grouping instructions 
# - The first entry of each tuple is the component type/name
# - The second is a boolean indicating whether the component is mandatory for all or optional
#
# For example, "opcode" is mandatory for all instructions in a group, as we can't group them otherwise. However,
# the auxiliary entry is not mandatory - in a given group, some instructions may have such an entry, while other do
# not. Those that lack an optional component will be treated as "default" encoding.
#
# Example of optional component:
#        0x90 : NOP
#   repz 0x90 : PAUSE
# In this case, the `repz` is an optional encoding component, meaning that its presence will cause PAUSE
# to be decoded, but its absence will cause NOP to be decoded.
#
# Example of mandatory component:
#   0x0F 0x00 /0:mem: SLDT Mw
#   0x0F 0x00 /0:reg: SLDT Rv
# In this case, all instructions must specify both the reg component (/0) and the mod (mem or reg). 
# If any of them is present for one instruction but absent for another, will lead to an error. For example, the 
# following spec will cause an error, since there's no grouping possible, because one of the instructions specifies 
# mem mode, while the other does not specify anything:
#   0x0F 0x00 /0:mem : SLDT Mw
#   0x0F 0x00 /0     : SLDT Rv
#
components_legacy = [
    GroupComponent('opcode',        256, True),
    GroupComponent('opcode_last',   256, True),
    GroupComponent('vendor',        16,  False),
    GroupComponent('prefix',        4,   True),
    GroupComponent('modrmreg',      8,   True),
    GroupComponent('modrmmod',      2,   True),
    GroupComponent('modrmrm',       8,   True),
    GroupComponent('mode',          4,   False),
    GroupComponent('asize',         4,   False),
    GroupComponent('dsize',         6,   False),
    GroupComponent('feature',       8,   False),
    GroupComponent('auxiliary',     10,  False),
    GroupComponent('w',             2,   True),
]

components_ex = [
    GroupComponent('mmmmm',         32,  True),
    GroupComponent('opcode',        256, True),
    GroupComponent('pp',            4,   True),
    GroupComponent('l',             4,   True),
    GroupComponent('w',             2,   True),
    GroupComponent('wi',            2,   True),
    GroupComponent('nd',            2,   True),
    GroupComponent('nf',            2,   True),
    GroupComponent('sc',            16,  True),
    GroupComponent('lpdf',          64,  True),
    GroupComponent('modrmreg',      8,   True),
    GroupComponent('modrmmod',      2,   True),
    GroupComponent('modrmrm',       8,   True),
]


component_value_index = {
    None    : 0,
    'None'  : 0,

    # modrm.mod
    'mem'   : 0,
    'reg'   : 1,

    # mandatory prefixes; using 'P' prefix so they're not confused with an opcode
    'PNP'   : 0,
    'P0x66' : 1,
    'P0xF3' : 2,
    'P0xF2' : 3,

    # other prefixes/redirection conditions
    'repz'  : 1,
    'rep'   : 2,
    'rexb'  : 3,
    'rexw'  : 4,
    'mo64'  : 5,
    'riprel': 6,
    'rex2'  : 7,
    'rex2w' : 8,

    # Mode
    'm16'   : 1,
    'm32'   : 2,
    'm64'   : 3,

    # Default address size
    'as16'  : 1,
    'as32'  : 2,
    'as64'  : 3,

    # Default data size
    'ds16'  : 1,
    'ds32'  : 2,
    'ds64'  : 3,
    'dd64'  : 4,
    'df64'  : 5,

    # Vendor redirection.
    'any'   : 0,
    'intel' : 1,
    'amd'   : 2,

    # Feature redirection.
    'mpx'   : 1,
    'cet'   : 2,
    'cldm'  : 3,
    'piti'  : 4,
    'movrs' : 5,
    'bhi'   : 6,
}


def compute_index(
    value: str
) -> int:
    """ 
    Given a component value, convert it to an index inside te C decoding table. Values which are present inside the 
    component_value_index dict will be translated using that. All other values will be considered to be hex values, 
    so they will be int(value, 16). The returned index is used when decoding an instruction, in order to lookup the 
    next viable entry in the multi-way decode tree.

    Parameters
    ----------
    value: str
        The index to be converted to an integer index.

    Returns
    -------
    An integer representing the index of the given decode component.

    Example
    -------
    Input: 
        value: repz
    Returns:
        4

    Input:
        value: 0xCC
    Returns:
        204
    """
    if value in component_value_index:
        return component_value_index[value]
    return int(value, 16)


def group_find_component(
    instructions: list[instrux.Instruction],
    components: list[GroupComponent]
) -> dict:
    """ 
    Given a list of instructions and a list of decoding components, return the decoding component that covers all the
    instructions in the list. For example, in an initial call to this function for the list of all legacy instructions
    the "opcode" component would be returned.

    Parameters
    ----------
    instructions: list[instrux.Instruction]
        The list of instructions to be grouped.
    components:
        The list of components used for grouping.

    Returns
    -------
    A dict representing the decode component that can be used to cover all instructions in the list.
    None if no such component could be found.
    """
    for c in components:
        if c.All:
            # Some components must be present for all instructions in the list - for example, opcode.
            bad = False
            for i in instructions:
                if not i.Encoding[c.Type]:
                    bad = True
                    break
        else:
            # Optional components need only be present for a single instruction in the list.
            bad = True
            for i in instructions:
                if i.Encoding[c.Type]:
                    bad = False
                    break
        if bad:
            continue
        return c

    # If we couldn't find a component, check if filters are present. Any entry in the encoding
    # that does not match a grouping component is considered a filter. Filters are used to make
    # decisions on the validty of an encoding during the decode process. This allows us to only
    # make the checks for the instructions that require them, instead of always doing them for
    # all the instructions.
    if len(i.Filters) > 0:
        return GroupComponent(i.Filters[0], 1, True, True)

    # No grouping component, and no filter found.
    return None


def group_instructions(
    instructions: list[instrux.Instruction],
    components: list[GroupComponent]
) -> dict:
    """ 
    Given a list of instructions and a list of decoding components, find the best grouping component, distribute all 
    instructions inside an array of children entries based on the identified grouping component, and recurse for all 
    children entries, until we are left with leaf entries only. A leaf entry is composed of a single Instruction 
    object.

    Parameters
    ----------
    instructions: list[instrux.Instruction]
        The list of instructions to be grouped.
    components:
        The list of components used for grouping.

    Returns
    -------
    A dictionary containing two keys: 
        - "component": indicates the component type used for the current grouping. It is one of "components_legacy"
          or "components_ex", depending on which was used for the grouping.
        - "children": an array of N entries, where each entry of the array contains an array of instructions that can 
          be further grouped. The size N of the "children" array is given by the number of possible entries for the 
          given "component". For example, an "opcode" component can have up to 256 values, so "children" will have 
          256 entries. A "modrmreg" component can have up to 8 values, so "children" will have 8 entries.

    Example
    -------
    Consider the following list of (simplified) initial instructions (only opcode and reduced encoding shown):
    [{"I1", "0xBD"}, {"I2", "0xCC"}, {"I4", "FF /1"}, {"I5", "FF /5"}]

    During the first call, "opcode" would be chosen to group the instructions, so we would end up with the following
    result:
    { 
        "component": "opcode",
        "children": [
            ... 
            Pos 0xBD: [{"I1", "0xBD"}],
            ...
            Pos 0xCC: [{"I2", "0xCC"}],
            ...
            Pos 0xFF: [{"I4", "FF /1"}, {"I5", "FF /5"}]
        ]
    }

    We would then recurse for each child in the children array. Note that for opcodes 0xBD and 0xCC, we already have
    leaf entries, so further grouping will not be required.

    For opcode 0xFF, further grouping is needed. At the next step, the "modrmreg" will be chosen for grouping, with
    the following result:
    { 
        "component": "modrmreg",
        "children": [
            Pos 0: []
            Pos 1: [{"I4", "FF /1"}]
            Pos 2: []
            Pos 3: []
            Pos 4: []
            Pos 5: [{"I5", "FF /5"}]
            Pos 6: []
            Pos 7: []
        ]
    }

    As in the previous example, we would recurse for each child, but we are already at leaf entries, so no more grouping
    is required.
    """
    
    # Find a good grouping component for the current instruction list.
    comp = group_find_component(instructions, components)

    # If no good component was found, we probably reached a leaf entry.
    if not comp and len(instructions) == 1:
        # Reached leaf entry, no more grouping needed.
        return InstructionGroup(None, instructions[0])
    elif not comp or (comp.Filter and len(instructions) != 1):
        # No grouping component found for multiple instructions - error.
        print("ERROR: Cannot properly group the following instructions. Please review specs!")
        for i in instructions: print("    -> ", i, " with encoding: ", i.RawEnc)
        raise Exception("Grouping error: invalid/incomplete specification!")

    # Allocate the sub-group array, based on the number of entries in the current group.
    group = InstructionGroup(comp, [])

    for i in range(0, comp.Size):
        group.Children.append([])

    # Now go through every instruction in the current group, and distribute it on its position.
    # Note that at each grouping step, we pop the used component from the instruction
    # encoding array, so that it's not used again.
    for i in instructions:
        if comp.Filter:
            index = 0
            i.Filters.pop(0)
        elif len(i.Encoding[comp.Type]) > 0:
            index = compute_index(i.Encoding[comp.Type].pop(0))
        else:
            index = 0
        group.Children[index].append(i)

    # Now recurse, and group every sub-group of instructions.
    for i in range(0, comp.Size):
        # Skip empty groups.
        if not group.Children[i]:
            continue

        # Recursively group instructions.
        group.Children[i] = group_instructions(group.Children[i], components)

    return group


def group_dump(
    group: InstructionGroup,
    level: int = 0
):
    """
    Dump the entire translation tree identified by the root "group".
    """
    if group.Component is None:
        print("    " * level, group.Children)
        return
    for i in range(0, len(group.Children)):
        if not group.Children[i]:
            continue
        print("    " * level, group.Component.Type, '%02x' % i)
        group_dump(group.Children[i], level + 1)
