from abc import ABC, abstractmethod

class Token(ABC):
    @abstractmethod
    def to_simple_string(self) -> str:
        pass

    def __repr__(self) -> str:
        return self.to_simple_string()

