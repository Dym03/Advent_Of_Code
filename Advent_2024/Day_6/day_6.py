import numpy as np


class GameField:
    def __init__(self, file):
        with open(file, "r") as f:
            x = f.read()
        listed = []
        for line in x.rsplit("\n"):
            listed.append([char for char in line])
        listed = np.array(listed)
        self.game_field = listed
        self.game_field_0 = self.game_field.copy()
        self.loc_0 = np.where(self.game_field == "^")
        self.loc_0 = [self.loc_0[0][0], self.loc_0[1][0]]
        self.loc = self.loc_0.copy()
        self.in_field = True
        self.locations = []
        self.options = 0

    def turn(self):
        if self.game_field[*self.loc] == "^":
            self.game_field[*self.loc] = ">"
        elif self.game_field[*self.loc] == ">":
            self.game_field[*self.loc] = "v"
        elif self.game_field[*self.loc] == "v":
            self.game_field[*self.loc] = "<"
        elif self.game_field[*self.loc] == "<":
            self.game_field[*self.loc] = "^"

    def move_ahead(self):
        if self.game_field[*self.loc] == "^":
            if self.loc[0] != 0:
                if self.game_field[self.loc[0] - 1, self.loc[1]] == "#":
                    self.turn()
                else:
                    self.game_field[*self.loc] = "X"
                    self.loc[0] -= 1
                    self.game_field[*self.loc] = "^"
        elif self.game_field[*self.loc] == ">":
            if self.loc[1] != self.game_field.shape[1] - 1:
                if self.game_field[self.loc[0], self.loc[1] + 1] == "#":
                    self.turn()
                else:
                    self.game_field[*self.loc] = "X"
                    self.loc[1] += 1
                    self.game_field[*self.loc] = ">"
        elif self.game_field[*self.loc] == "v":
            if self.loc[1] != self.game_field.shape[1] - 1:
                if self.game_field[self.loc[0] + 1, self.loc[1]] == "#":
                    self.turn()
                else:
                    self.game_field[*self.loc] = "X"
                    self.loc[0] += 1
                    self.game_field[*self.loc] = "v"
        elif self.game_field[*self.loc] == "<":
            if self.loc[1] != 0:
                if self.game_field[self.loc[0], self.loc[1] - 1] == "#":
                    self.turn()
                else:
                    self.game_field[*self.loc] = "X"
                    self.loc[1] -= 1
                    self.game_field[*self.loc] = "<"

    def in_game_field(self):
        return (
            0 < self.loc[0] < self.game_field.shape[0] - 1
            and 0 < self.loc[1] < self.game_field.shape[1] - 1
        )

    def play_game(self):
        while self.in_game_field():
            self.move_ahead()
            if [self.loc[0], self.loc[1]] not in self.locations:
                self.locations.append([self.loc[0], self.loc[1]])

    def sol_2(self):
        for i, loc in enumerate(self.locations):
            if loc == self.loc_0:
                continue
            self.ocurance = np.zeros(shape=self.game_field.shape)
            self.ocurance.fill(0)
            self.game_field = self.game_field_0.copy()
            self.game_field[*loc] = "#"
            self.loc = self.loc_0.copy()
            while self.in_game_field():
                self.ocurance[*self.loc] += 1
                if self.ocurance[*self.loc] > 4:
                    self.options += 1
                    # print(self.game_field)
                    break
                self.move_ahead()


if __name__ == "__main__":
    file = "test.txt"
    x = GameField(file)
    x.play_game()
    x.sol_2()
    print(x.options)
