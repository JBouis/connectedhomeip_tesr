# Copyright (c) 2021 Project CHIP Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
from enum import Enum, auto

from .gn import GnBuilder


class stm32App(Enum):
    LIGHT = auto()

    def ExampleName(self):
        if self == stm32App.LIGHT:
            return 'lighting-app'
        else:
            raise Exception('Unknown app type: %r' % self)

    def AppNamePrefix(self):
        if self == stm32App.LIGHT:
            return 'chip-stm32-lighting-example'
        else:
            raise Exception('Unknown app type: %r' % self)

    def FlashBundleName(self):
        if self == stm32App.LIGHT:
            return 'lighting_app.out.flashbundle.txt'
        else:
            raise Exception('Unknown app type: %r' % self)

    def BuildRoot(self, root):
        return os.path.join(root, 'examples', self.ExampleName(), 'stm32')


class stm32Board(Enum):
    stm32 = 1

    def GnArgName(self):
        if self == stm32Board.stm32:
            return 'STM32WB5MM-DK'
        else:
            raise Exception('Unknown board #: %r' % self)


class stm32Builder(GnBuilder):

    def __init__(self,
                 root,
                 runner,
                 app: stm32App = stm32App.LIGHT,
                 board: stm32Board = stm32Board.stm32,
                 enable_rpcs: bool = False):
        super(stm32Builder, self).__init__(
            root=app.BuildRoot(root),
            runner=runner)
        self.app = app
        self.board = board
        self.enable_rpcs = enable_rpcs

    def GnBuildArgs(self):
        args = ['stm32_target_ic=\"%s\"' % self.board.GnArgName()]
       
        return args

    def build_outputs(self):
        items = {}
        for extension in ["out", "out.map", "out.hex"]:
            name = '%s.%s' % (self.app.AppNamePrefix(), extension)
            items[name] = os.path.join(self.output_dir, name)

        # Figure out flash bundle files and build accordingly
        with open(os.path.join(self.output_dir, self.app.FlashBundleName())) as f:
            for line in f.readlines():
                name = line.strip()
                items['flashbundle/%s' %
                      name] = os.path.join(self.output_dir, name)

        return items
