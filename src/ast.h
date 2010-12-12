#ifndef KUNGJS_AST_H_
#define KUNGJS_AST_H_

#include <boost/tr1/memory.hpp>
#include <vector>

namespace kungjs {
namespace ast {

class Node {
 public:
  virtual ~Node();
  //virtual visit(const Compiler& compiler) = 0;
};

template <typename T>
class LiteralNode : public Node {
 public:
  explicit LiteralNode(T value)
      : value_(value) {}

  T value() {
    return value_;
  }

  static std::tr1::shared_ptr<LiteralNode<T> > create(T value) {
    return std::tr1::shared_ptr<LiteralNode<T> >(new LiteralNode<T>(value));
  }

 private:
  T value_;
};

class KeywordNode : public Node {
 public:
  explicit KeywordNode(std::string value)
      : value_(value) {}

  const std::string value() const {
    return value_;
  }

 private:
  std::string value_;
};

class IdentifierNode : public Node {
 public:
  explicit IdentifierNode(std::string value)
      : value_(value) {}

  const std::string value() const {
    return value_;
  }

  static std::tr1::shared_ptr<IdentifierNode> create(std::string value) {
    return std::tr1::shared_ptr<IdentifierNode>(new IdentifierNode(value));
  }

 private:
  std::string value_;
};

class NullNode : public LiteralNode<std::string> {
 public:
  NullNode() : LiteralNode<std::string>("null") {}
};

class ThisNode : public Node {};

class DeleteNode : public Node {
 public:
  explicit DeleteNode(std::tr1::shared_ptr<Node> operand)
      : operand_(operand) {}

  const std::tr1::shared_ptr<Node> operand() const {
    return operand_;
  }

  static std::tr1::shared_ptr<DeleteNode> create(std::tr1::shared_ptr<Node> operand) {
    return std::tr1::shared_ptr<DeleteNode>(new DeleteNode(operand));
  }

 private:
  std::tr1::shared_ptr<Node> operand_;
};

class PreDecrementNode : public Node {
 public:
  explicit PreDecrementNode(std::tr1::shared_ptr<Node> operand)
      : operand_(operand) {}

  const std::tr1::shared_ptr<Node> operand() const {
    return operand_;
  }

  static std::tr1::shared_ptr<PreDecrementNode> create(std::tr1::shared_ptr<Node> operand) {
    return std::tr1::shared_ptr<PreDecrementNode>(new PreDecrementNode(operand));
  }

 private:
  std::tr1::shared_ptr<Node> operand_;
};

class PreIncrementNode : public Node {
 public:
  explicit PreIncrementNode(std::tr1::shared_ptr<Node> operand)
      : operand_(operand) {}

  const std::tr1::shared_ptr<Node> operand() const {
    return operand_;
  }

  static std::tr1::shared_ptr<PreIncrementNode> create(std::tr1::shared_ptr<Node> operand) {
    return std::tr1::shared_ptr<PreIncrementNode>(new PreIncrementNode(operand));
  }

 private:
  std::tr1::shared_ptr<Node> operand_;
};

class PostDecrementNode : public Node {
 public:
  explicit PostDecrementNode(std::tr1::shared_ptr<Node> operand)
      : operand_(operand) {}

  const std::tr1::shared_ptr<Node> operand() const {
    return operand_;
  }

  static std::tr1::shared_ptr<PostDecrementNode> create(std::tr1::shared_ptr<Node> operand) {
    return std::tr1::shared_ptr<PostDecrementNode>(new PostDecrementNode(operand));
  }

 private:
  std::tr1::shared_ptr<Node> operand_;
};

class PostIncrementNode : public Node {
 public:
  explicit PostIncrementNode(std::tr1::shared_ptr<Node> operand)
      : operand_(operand) {}

  const std::tr1::shared_ptr<Node> operand() const {
    return operand_;
  }

  static std::tr1::shared_ptr<PostIncrementNode> create(std::tr1::shared_ptr<Node> operand) {
    return std::tr1::shared_ptr<PostIncrementNode>(new PostIncrementNode(operand));
  }

 private:
  std::tr1::shared_ptr<Node> operand_;
};

class NotNode : public Node {
 public:
  explicit NotNode(std::tr1::shared_ptr<Node> operand)
      : operand_(operand) {}

  const std::tr1::shared_ptr<Node> operand() const {
    return operand_;
  }

  static std::tr1::shared_ptr<NotNode> create(std::tr1::shared_ptr<Node> operand) {
    return std::tr1::shared_ptr<NotNode>(new NotNode(operand));
  }

 private:
  std::tr1::shared_ptr<Node> operand_;
};

class NewNode : public Node {
 public:
  explicit NewNode(std::tr1::shared_ptr<Node> operand)
      : operand_(operand) {}

  const std::tr1::shared_ptr<Node> operand() const {
    return operand_;
  }

  static std::tr1::shared_ptr<NewNode> create(std::tr1::shared_ptr<Node> operand) {
    return std::tr1::shared_ptr<NewNode>(new NewNode(operand));
  }

 private:
  std::tr1::shared_ptr<Node> operand_;
};

class BinaryOperatorNode : public Node {
 public:
  BinaryOperatorNode(std::string oper,
                     std::tr1::shared_ptr<Node> lhs,
                     std::tr1::shared_ptr<Node> rhs)
      : oper_(oper), lhs_(lhs), rhs_(rhs) {}

  const std::string oper() const {
    return oper_;
  }

  const std::tr1::shared_ptr<Node> lhs() const {
    return lhs_;
  }

  const std::tr1::shared_ptr<Node> rhs() const {
    return rhs_;
  }

  static std::tr1::shared_ptr<BinaryOperatorNode> create(std::string oper,
                                                         std::tr1::shared_ptr<Node> lhs,
                                                         std::tr1::shared_ptr<Node> rhs) {
    return std::tr1::shared_ptr<BinaryOperatorNode>(new BinaryOperatorNode(oper, lhs, rhs));
  }

 private:
  std::string oper_;
  std::tr1::shared_ptr<Node> lhs_;
  std::tr1::shared_ptr<Node> rhs_;
};

class AssignmentNode : public Node {
 public:
  AssignmentNode(std::string oper,
                 std::tr1::shared_ptr<Node> lhs,
                 std::tr1::shared_ptr<Node> rhs)
      : oper_(oper), lhs_(lhs), rhs_(rhs) {}

  const std::string oper() const {
    return oper_;
  }

  const std::tr1::shared_ptr<Node> lhs() const {
    return lhs_;
  }

  const std::tr1::shared_ptr<Node> rhs() const {
    return rhs_;
  }

  static std::tr1::shared_ptr<AssignmentNode> create(std::string oper,
                                                     std::tr1::shared_ptr<Node> lhs,
                                                     std::tr1::shared_ptr<Node> rhs) {
    return std::tr1::shared_ptr<AssignmentNode>(new AssignmentNode(oper, lhs, rhs));
  }

 private:
  std::string oper_;
  std::tr1::shared_ptr<Node> lhs_;
  std::tr1::shared_ptr<Node> rhs_;
};

class EmptyNode : public Node {};

typedef std::tr1::shared_ptr<Node> PNode;
typedef std::vector<PNode> Nodes;

class BlockNode : public Node {
 public:
  BlockNode(Nodes statements)
      : statements_(statements) {}

  Nodes statements() const {
    return statements_;
  }

 private:
  Nodes statements_;
};

class ExpressionNode : public Node {
 public:
  ExpressionNode(std::tr1::shared_ptr<Node> child, Nodes children)
      : children_(children) {
        children_.push_back(child);
      }

  static std::tr1::shared_ptr<ExpressionNode> create(std::tr1::shared_ptr<Node> child, Nodes children) {
    return std::tr1::shared_ptr<ExpressionNode>(new ExpressionNode(child, children));
  }

 private:
  Nodes children_;
};

class ProgramNode : public Node {
 public:
  ProgramNode(Nodes statements)
      : statements_(statements) {}

  Nodes statements() const {
    return statements_;
  }

 private:
  Nodes statements_;
};

class FunctionNode : public Node {};

} // namespace ast
} // namespace kungjs

#endif // KUNGJS_AST_H_

