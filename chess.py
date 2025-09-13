
import chess
from chessboard import display
import time

class State:
    def __init__(self, board=None):
       
        if board is None:
            self.board = chess.Board()
        else:
            self.board = board

    def isTerminal(self):
        return self.board.is_game_over()

    def moveGen(self):
        
        children = []
        for move in self.board.legal_moves:
            new_board = self.board.copy()
            new_board.push(move)
            children.append(State(new_board))
        return children

    def __str__(self):
        return str(self.board)

    def __eq__(self, other):
        return self.board.fen() == other.board.fen()

    def __hash__(self):
        return hash(self.board.fen())

    def evaluate(self):
       
        if self.board.is_checkmate():
  
            if self.board.turn == chess.WHITE:
                return -10000
            else:
                return 10000

        
        if (
            self.board.is_stalemate()
            or self.board.is_insufficient_material()
            or self.board.can_claim_draw()
        ):
            return 0

        score = 0.0

       
        piece_values = {
            chess.PAWN: 1.0,
            chess.KNIGHT: 4.0,
            chess.BISHOP: 4.0,
            chess.ROOK: 6.0,
            chess.QUEEN: 10.0,
            chess.KING: 200.0,
        }

        for sq, piece in self.board.piece_map().items():
            value = piece_values.get(piece.piece_type, 0)
            if piece.color == chess.WHITE:
                score += value
            else:
                score -= value

  
        center_squares = [chess.D4, chess.E4, chess.D5, chess.E5]
        for sq in center_squares:
            piece = self.board.piece_at(sq)
            if piece:
                if piece.color == chess.WHITE:
                    score += 0.2
                else:
                    score -= 0.2

      
        b = self.board.copy()
        b.turn = chess.WHITE
        white_moves = len(list(b.legal_moves))
        b.turn = chess.BLACK
        black_moves = len(list(b.legal_moves))
        score += 0.05 * (white_moves - black_moves)

    
        white_king_sq = self.board.king(chess.WHITE)
        black_king_sq = self.board.king(chess.BLACK)

        if white_king_sq is not None:
            attackers = self.board.attackers(chess.BLACK, white_king_sq)
            score -= 0.5 * len(attackers)

        if black_king_sq is not None:
            attackers = self.board.attackers(chess.WHITE, black_king_sq)
            score += 0.5 * len(attackers)

        return score


def minimax(state, depth, alpha, beta, maximizingPlayer, maxDepth):
   
    if state.isTerminal() or depth == maxDepth:
        return state.evaluate(), None

    best_move = None

    if maximizingPlayer:  
        maxEval = float('-inf')
        for child in state.moveGen():
            eval_score, _ = minimax(child, depth + 1, alpha, beta, False, maxDepth)

            if eval_score > maxEval:
                maxEval = eval_score
                
                best_move = child.board.peek()

            alpha = max(alpha, eval_score)
            if alpha >= beta:
                break 

        return maxEval, best_move

    else:  
        minEval = float('inf')
        for child in state.moveGen():
            eval_score, _ = minimax(child, depth + 1, alpha, beta, True, maxDepth)

            if eval_score < minEval:
                minEval = eval_score
                best_move = child.board.peek()

            beta = min(beta, eval_score)
            if alpha >= beta:
                break

        return minEval, best_move


def play_game():
    current_state = State()  
    maxDepth =3
    game_board = display.start()  

    print("Artificial Intelligence – Assignment 3")
    print("Simple Chess AI")
    print("You are playing as White (enter moves in UCI format, e.g., e2e4)")

    while not current_state.isTerminal():
        
        display.update(current_state.board.fen(), game_board)

        
        if display.check_for_quit():
            break

        if current_state.board.turn == chess.WHITE:  
            try:
                move_uci = input("Enter your move (e.g., e2e4, g1f3, a7a8q) or 'quit': ")

                if move_uci.lower() == 'quit':
                    break

                move = chess.Move.from_uci(move_uci)

                if move in current_state.board.legal_moves:
                    new_board = current_state.board.copy()
                    new_board.push(move)
                    current_state = State(new_board)
                else:
                    print("Invalid move! Try again.")
                    continue
            except ValueError:
                print("Invalid input format! Use UCI format like 'e2e4'.")
                continue
        else:  
            print("AI is thinking...")
            start_time = time.time()

    
            eval_score, best_move = minimax(current_state, 0, float('-inf'), float('inf'), False, maxDepth)
            end_time = time.time()

            print(f"AI thought for {end_time - start_time:.2f} seconds")

            if best_move:
                new_board = current_state.board.copy()
                new_board.push(best_move)
                current_state = State(new_board)
                print(f"AI plays: {best_move.uci()}")
            else:
      
                legal_moves = list(current_state.board.legal_moves)
                if legal_moves:
                    move = legal_moves[0]
                    new_board = current_state.board.copy()
                    new_board.push(move)
                    current_state = State(new_board)
                    print(f"AI plays (fallback): {move.uci()}")
                else:
                    break

 
    print("\nGame over!")
    display.update(current_state.board.fen(), game_board)

    if current_state.board.is_checkmate():
        winner= "White" if current_state.board.turn == chess.BLACK else "Black"
        print("Checkmate! " + winner + " wins!")
    elif current_state.board.is_stalemate():
        print("Stalemate! It's a draw.")
    elif current_state.board.is_insufficient_material():
        print("Insufficient material! It's a draw.")
    elif current_state.board.can_claim_draw():
        print("Draw by repetition or 50-move rule!")

 
    time.sleep(3)
    display.terminate()


if __name__ == "__main__":
    play_game()