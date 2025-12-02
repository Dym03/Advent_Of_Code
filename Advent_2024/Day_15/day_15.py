import numpy as np
from collections import deque


class GameField:
    def __init__(self, file):
        self.gamefield = []
        self.instructions = ""
        self.load_file(file)
        self.loc_0 = [int(x.squeeze()) for x in np.where(self.gamefield == "@")]
        self.boxes_in_front = []
        self.play_game()
        self.calc_gps()

    def reprint_loc(self):
        self.gamefield[*self.loc_0] = "."

    def load_file(self, file):
        with open(file, "r") as f:
            for line in f.readlines():
                if line.startswith("#"):
                    self.gamefield.append([char for char in line.strip()])
                elif line == []:
                    continue
                else:
                    self.instructions += line.strip()
        self.gamefield = np.array(self.gamefield)
        self.rows, self.columns = self.gamefield.shape

    def decode_instruction(self, direction):
        directions = ["<", ">", "v", "^"]
        delta = [[0, -1], [0, 1], [1, 0], [-1, 0]]
        for i, char in enumerate(directions):
            if direction == char:
                return delta[i]

    def check_for_box(self, delta):
        if self.gamefield[self.loc_0[0] + delta[0], self.loc_0[1] + delta[1]] == "O":
            return True
        else:
            return False

    def check_for_side(self, delta):
        if self.gamefield[self.loc_0[0] + delta[0], self.loc_0[1] + delta[1]] == "#":
            return True
        else:
            return False

    def find_boxes_in_front(self, delta):
        self.boxes_in_front = []
        while self.check_for_box(
            [(len(self.boxes_in_front) + 1) * char for char in delta]
        ):
            self.boxes_in_front.append(
                [
                    self.loc_0[0] + (len(self.boxes_in_front) + 1) * delta[0],
                    self.loc_0[1] + (len(self.boxes_in_front) + 1) * delta[1],
                ]
            )

    def play_game(self):
        for char in self.instructions:
            direction = self.decode_instruction(char)
            # print(f'i look for boxes in direciton {char}')
            # print(f'delta for {char} is {direction}')
            self.find_boxes_in_front(direction)
            # print(f'I have {len(self.boxes_in_front)} boxes in front of me')

            if self.check_for_side(
                [(len(self.boxes_in_front) + 1) * element for element in direction]
            ):
                continue
            else:
                if len(self.boxes_in_front) > 0:
                    self.gamefield[
                        self.boxes_in_front[-1][0] + direction[0],
                        self.boxes_in_front[-1][1] + direction[1],
                    ] = "O"
                self.reprint_loc()
                self.loc_0 = [x + y for x, y in zip(self.loc_0, direction)]
                # print(self.loc_0)
                self.gamefield[*self.loc_0] = "@"
            # print(self.gamefield)

    def calc_gps(self):
        summ = 0
        for i in range(0, self.rows):
            for j in range(0, self.columns):
                if self.gamefield[i, j] == "O":
                    summ += 100 * i + j
        print(summ)


class GameField2:
    def __init__(self, file):
        self.gamefield = []
        self.instructions = ""
        self.load_file(file)
        # print(self.gamefield)
        self.loc_0 = [int(x.squeeze()) for x in np.where(self.gamefield == "@")]
        self.boxes_in_front = []
        self.play_game()
        self.calc_gps()

    def reprint_loc(self):
        self.gamefield[*self.loc_0] = "."

    def load_file(self, file):
        with open(file, "r") as f:
            for line in f.readlines():
                if line.startswith("#"):
                    list_for_line = []
                    for char in line.strip():
                        if char == "@":
                            list_for_line.extend(["@", "."])
                        elif char == "O":
                            list_for_line.extend(["[", "]"])
                        else:
                            list_for_line.extend([char, char])
                    self.gamefield.append(list_for_line)
                elif line == []:
                    continue
                else:
                    self.instructions += line.strip()
        self.gamefield = np.array(self.gamefield)
        self.rows, self.columns = self.gamefield.shape

    def decode_instruction(self, direction):
        directions = ["<", ">", "v", "^"]
        delta = [[0, -1], [0, 1], [1, 0], [-1, 0]]
        for i, char in enumerate(directions):
            if direction == char:
                return delta[i]

    def check_for_side(self, delta):
        locations = [self.loc_0] + self.boxes_in_front
        for location in locations:
            if self.gamefield[location[0] + delta[0], location[1] + delta[1]] == "#":
                return True
            else:
                continue
        return False

    def check_ahead(self, delta):
        que = deque([[self.loc_0[0], self.loc_0[1]]])
        possible_boxes_ahead = []
        while que:
            x, y = que.pop()
            if delta == [0, 1] or delta == [0, -1]:
                if self.gamefield[x + delta[0], y + delta[1]] == "[":
                    possible_boxes_ahead.append([x + delta[0], y + delta[1]])
                    possible_boxes_ahead.append([x + delta[0], y + delta[1] + 1])
                    que.append([x + delta[0], y + 2 * delta[1]])
                elif self.gamefield[x + delta[0], y + delta[1]] == "]":
                    possible_boxes_ahead.append([x + delta[0], y + delta[1]])
                    possible_boxes_ahead.append([x + delta[0], y + delta[1] - 1])
                    que.append([x + delta[0], y + 2 * delta[1]])
            else:
                if self.gamefield[x + delta[0], y + delta[1]] == "]":
                    possible_boxes_ahead.append([x + delta[0], y + delta[1] - 1])
                    possible_boxes_ahead.append([x + delta[0], y + delta[1]])
                    que.append([x + delta[0], y + delta[1]])
                    que.append([x + delta[0], y + delta[1] - 1])
                elif self.gamefield[x + delta[0], y + delta[1]] == "[":
                    possible_boxes_ahead.append([x + delta[0], y + delta[1]])
                    possible_boxes_ahead.append([x + delta[0], y + delta[1] + 1])
                    que.append([x + delta[0], y + delta[1]])
                    que.append([x + delta[0], y + delta[1] + 1])

                # print(que)
            for box in possible_boxes_ahead:
                if box not in self.boxes_in_front:
                    self.boxes_in_front.append(box)
        # print(self.boxes_in_front)

    def move_ahead(self, delta):
        locations = [self.loc_0] + self.boxes_in_front
        if delta == [0, -1]:
            locations = sorted(locations, key=lambda x: x[1])
        elif delta == [0, 1]:
            locations = sorted(locations, key=lambda x: x[1])[::-1]
        elif delta == [1, 0]:
            locations = sorted(locations, key=lambda x: x[0])[::-1]
        elif delta == [-1, 0]:
            locations = sorted(locations, key=lambda x: x[0])

        for location in locations:
            self.gamefield[location[0] + delta[0], location[1] + delta[1]] = (
                self.gamefield[location[0], location[1]]
            )
            self.gamefield[location[0], location[1]] = "."

    def play_game(self):
        for i, char in enumerate(self.instructions):
            direction = self.decode_instruction(char)
            self.check_ahead(direction)
            if self.check_for_side(direction):
                self.boxes_in_front = []
                # print(self.gamefield)
                continue
            else:
                

                self.move_ahead(direction)
                self.reprint_loc()
                self.loc_0 = [x + y for x, y in zip(self.loc_0, direction)]
                self.gamefield[*self.loc_0] = "@"
            self.boxes_in_front = []
            
        for row in self.gamefield:
            print("".join(row) + "\n")

    def calc_gps(self):
        summ = 0
        for i in range(0, self.rows):
            for j in range(0, self.columns):
                if self.gamefield[i, j] == "[":
                    summ += 100 * i + j
        print(summ)
        with open(f"result.txt", "a") as txt_file:
            for line in self.gamefield:
                txt_file.write("".join(line) + "\n")


if __name__ == "__main__":
    file = "michal.txt"
    x = GameField2(file)
