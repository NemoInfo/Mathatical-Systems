"""Heat Equation Solver Class"""

import numpy as np
from typing import Callable, Union
from scipy.linalg import solve_banded 
import attrs


@attrs.define
class HeatSolver:
    dx: np.float32
    dt: np.float32
    L: np.float32 = np.float32(100.0)
    k: np.float32 = np.float32(0.875)
    t_total: np.float32 = np.float32(600.0)
    T_x0: Callable[Union[np.ndarray, np.float32], Union[np.ndarray, np.float32]] \
        = lambda x: 500.0 * np.ones_like(x)
    T_0t: np.float32 = np.float32(0.0)
    T_Lt: np.float32 = np.float32(0.0)

    @property
    def xs(self) -> np.ndarray:
        return np.arange(0, self.L + self.dx - 1e-4, self.dx)

    @property
    def ts(self) -> np.ndarray:
        return np.arange(0, self.t_total + self.dt - 1e-4, self.dt)

    def _solve_analytic_x_t(self, x: np.ndarray, t: np.ndarray, N: int = 100_001) -> np.ndarray:
        x = np.atleast_1d(x)
        t = np.atleast_1d(t)

        ns = np.arange(1, N + 1, 2)
        sums = (1 / ns) * np.sin(ns[None, :] * np.pi * x[:, None] / self.L) @ \
            np.exp(- ns[:, None]**2 * np.pi**2 * self.k * t[None, :] / self.L**2)

        return 2000 / np.pi * sums.T

    def solve_analytic(self) -> np.ndarray:
        return self._solve_analytic_x_t(self.xs, self.ts)

    def solve_finite_distance(self) -> np.ndarray:
        A = self.k * self.dt / self.dx**2
        B = A * (self.dx**2 / (self.k * self.dt) - 2)

        N_x = int(self.L / self.dx + 1)        # Number of nodes per time step
        N_t = int(self.t_total / self.dt + 1)  # Number of nodes per position

        T = np.zeros((N_t, N_x))
        T[0, :] = self.T_x0(self.xs)
        T[:, 0] = self.T_0t
        T[:, -1] = self.T_Lt

        for j in range(0, N_t - 1):
            for i in range(1, N_x - 1):
                T[j+1, i] = A * (T[j, i+1] + T[j, i-1]) + B * T[j, i]

        return T

    def solve_crank_nicholson(self) -> np.ndarray:
        lmbd = (2 * self.dx**2) / (self.k * self.dt)
        alph = - lmbd - 2
        beta = - lmbd + 2
        N_x = int(self.L / self.dx) - 1
        N_t = int(self.t_total / self.dt) + 1

        # A = np.diag([alph] * N_x) + np.diag([1] * (N_x - 1), k=1) + \
        #     np.diag([1] * (N_x - 1), k=-1)
        Ab = np.zeros((3, N_x))
        Ab[0, 1:] = [1] * (N_x - 1)         
        Ab[1, :] = [alph] * N_x
        Ab[2, :-1] = [1] * (N_x-1)
      
        B = np.diag([beta] * N_x) + np.diag([-1] * (N_x - 1), k=1) + \
            np.diag([-1] * (N_x - 1), k=-1)

        T = np.zeros((N_t, N_x + 2))
        T[0, :] = self.T_x0(self.xs)
        T[:, 0] = self.T_0t
        T[:, -1] = self.T_Lt

        for j in range(N_t - 1):
            T[j+1, 1:-1] = solve_banded((1,1), Ab, B @ T[j, 1:-1])

        return T

    def solve_backward_distance(self) -> np.ndarray:
        lmbd = self.k * self.dt / (self.dx**2)
        alph = -(2 * lmbd + 1)
        N_x = int(self.L / self.dx) - 1
        N_t = int(self.t_total / self.dt)
        N = N_x*N_t

        A = np.diag([alph] * N) + np.diag([1] * (N-N_x), k=-N_x)
        A_T_im1_j = np.tile([lmbd] * (N_x - 1) + [0], N_t)
        A_T_ip1_j = np.tile([0] + [lmbd] * (N_x - 1), N_t)
        A += np.diag(A_T_im1_j[:-1], k=1) + np.diag(A_T_im1_j[:-1], k=-1)

        B = np.zeros(N) + (A_T_im1_j - lmbd) * \
            self.T_0t + (A_T_ip1_j - lmbd) * self.T_Lt
        B[:N_x] -= self.T_x0(np.arange(self.dx, self.L, self.dx))

        T = np.linalg.solve(A, B).reshape((N_t, N_x))
        T = np.insert(T, 0, self.T_x0(np.arange(self.dx, self.L, self.dx)), axis=0)
        T = np.insert(T, 0, self.T_0t, axis=1)
        T = np.insert(T, N_x + 1, self.T_Lt, axis=1)

        return T
