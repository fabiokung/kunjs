#include "kunjs/printer.h"
#include "kunjs/ast.h"

#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <iostream>
#include <string>

namespace kunjs {

namespace {

std::string Indent(int level) {
  std::string result;
  for (int i=0; i < level; i++)
    result.append(" ");

  return result;
}

}


ASTPrinter::ASTPrinter(int indent) : indentation(indent) {}

void ASTPrinter::operator()(std::vector<ast::Statement> const& list) const {
  std::for_each(list.begin(), list.end(), *this);
}

void ASTPrinter::operator()(ast::Program const& program) const {
  ASTPrinter printer(indentation + INDENT_STEP);
  std::cout << Indent(indentation) << "(Program " << std::endl;
  std::for_each(program.begin(), program.end(), printer);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ASTPrinter::operator()(ast::SourceElement const& element) const {
  ASTPrinter printer(indentation + INDENT_STEP);
  std::cout << Indent(indentation) << "(SourceElement " << std::endl;
  boost::apply_visitor(printer, element);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ASTPrinter::operator()(ast::FunctionDeclaration const& function) const {
  std::cout << Indent(indentation) << "(FunctionDeclaration)" << std::endl;
}

void ASTPrinter::operator()(ast::Statement const& statement) const {
  StatementPrinter printer(indentation + INDENT_STEP);
  std::cout << Indent(indentation) << "(Statement " << std::endl;
  boost::apply_visitor(printer, statement);
  std::cout << Indent(indentation) << ")" << std::endl;
}


StatementPrinter::StatementPrinter(int indent) : indentation(indent) {}

void StatementPrinter::operator()(ast::Expression const& expression) {
  std::cout << Indent(indentation) << "(Expression " << std::endl;
  ExpressionPrinter printer(indentation + INDENT_STEP);
  std::for_each(expression.begin(), expression.end(), printer);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::Var const& var) {
  for (ast::Var::const_iterator it = var.begin(); it != var.end(); ++it) {
    (*this)(*it);
  }
}

void StatementPrinter::operator()(ast::VarDeclaration const& declaration) {
  int next_level = indentation + INDENT_STEP;
  ExpressionPrinter print_expression(next_level);
  std::cout << Indent(indentation) << "(Var" << std::endl;

  std::cout << Indent(next_level) << "(name " << declaration.name << ")" << std::endl;
  if (declaration.assignment) print_expression(declaration.assignment.get());

  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::Noop const& noop) {
  std::cout << Indent(indentation) << "(Empty)" << std::endl;
}

void StatementPrinter::operator()(ast::If const& conditional) {
  int next = indentation + INDENT_STEP;
  StatementPrinter print_expression(next + INDENT_STEP);
  ASTPrinter print_statement(next + INDENT_STEP);

  std::cout << Indent(indentation) << "(If " << std::endl;

  std::cout << Indent(next) << "(condition " << std::endl;
  print_expression(conditional.condition);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(next) << "(true_clause " << std::endl;
  print_statement(conditional.true_clause);
  std::cout << Indent(next) << ")" << std::endl;

  if (conditional.false_clause) {
    std::cout << Indent(next) << "(false_clause " << std::endl;
    print_statement(conditional.false_clause.get());
    std::cout << Indent(next) << ")" << std::endl;
  }

  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::DoWhile const& loop) {
  int next = indentation + INDENT_STEP;
  StatementPrinter print_expression(next + INDENT_STEP);
  ASTPrinter print_statement(next + INDENT_STEP);

  std::cout << Indent(indentation) << "(DoWhile " << std::endl;

  std::cout << Indent(next) << "(body " << std::endl;
  print_statement(loop.statement);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(next) << "(condition " << std::endl;
  print_expression(loop.condition);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::While const& loop) {
  int next = indentation + INDENT_STEP;
  StatementPrinter print_expression(next + INDENT_STEP);
  ASTPrinter print_statement(next + INDENT_STEP);

  std::cout << Indent(indentation) << "(While " << std::endl;

  std::cout << Indent(next) << "(condition " << std::endl;
  print_expression(loop.condition);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(next) << "(body " << std::endl;
  print_statement(loop.statement);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::For const& loop) {
  int next = indentation + INDENT_STEP;
  StatementPrinter print_expression(next + INDENT_STEP);
  ASTPrinter print_statement(next + INDENT_STEP);

  std::cout << Indent(indentation) << "(For " << std::endl;

  if (loop.initialization) {
    std::cout << Indent(next) << "(initialization " << std::endl;
    print_expression(loop.initialization.get());
    std::cout << Indent(next) << ")" << std::endl;
  }

  if (loop.condition) {
    std::cout << Indent(next) << "(condition " << std::endl;
    print_expression(loop.condition.get());
    std::cout << Indent(next) << ")" << std::endl;
  }

  if (loop.action) {
    std::cout << Indent(next) << "(action " << std::endl;
    print_expression(loop.action.get());
    std::cout << Indent(next) << ")" << std::endl;
  }

  std::cout << Indent(next) << "(body " << std::endl;
  print_statement(loop.statement);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::ForWithVar const& loop) {
  int next = indentation + INDENT_STEP;
  StatementPrinter print_expression(next + INDENT_STEP);
  ASTPrinter print_statement(next + INDENT_STEP);

  std::cout << Indent(indentation) << "(ForWithVar " << std::endl;

  for (ast::Var::const_iterator it = loop.initialization.begin(); it != loop.initialization.end(); ++it) {
    std::cout << Indent(next) << "(initialization " << std::endl;
    print_expression(*it);
    std::cout << Indent(next) << ")" << std::endl;
  }

  if (loop.condition) {
    std::cout << Indent(next) << "(condition " << std::endl;
    print_expression(loop.condition.get());
    std::cout << Indent(next) << ")" << std::endl;
  }

  if (loop.action) {
    std::cout << Indent(next) << "(action " << std::endl;
    print_expression(loop.action.get());
    std::cout << Indent(next) << ")" << std::endl;
  }

  std::cout << Indent(next) << "(body " << std::endl;
  print_statement(loop.statement);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::Foreach const& loop) {
  int next = indentation + INDENT_STEP;
  ExpressionPrinter print_lhs_expression(next + INDENT_STEP);
  StatementPrinter print_expression(next + INDENT_STEP);
  ASTPrinter print_statement(next + INDENT_STEP);

  std::cout << Indent(indentation) << "(Foreach " << std::endl;

  std::cout << Indent(next) << "(item " << std::endl;
  print_lhs_expression(loop.item);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(next) << "(list " << std::endl;
  print_expression(loop.list);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(next) << "(body " << std::endl;
  print_statement(loop.statement);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::ForeachWithVar const& loop) {
  int next = indentation + INDENT_STEP;
  StatementPrinter print_expression(next + INDENT_STEP);
  ASTPrinter print_statement(next + INDENT_STEP);

  std::cout << Indent(indentation) << "(ForeachWithVar " << std::endl;

  std::cout << Indent(next) << "(item " << std::endl;
  print_expression(loop.item);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(next) << "(list " << std::endl;
  print_expression(loop.list);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(next) << "(body " << std::endl;
  print_statement(loop.statement);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::Continue const& node) {
  std::cout << Indent(indentation) << "(Continue";
  if (node.label) {
    std::cout << " (label " << node.label.get() << "))" << std::endl;
  } else {
    std::cout << ")" << std::endl;
  }
}

void StatementPrinter::operator()(ast::Break const& node) {
  std::cout << Indent(indentation) << "(Break";
  if (node.label) {
    std::cout << " (label " << node.label.get() << "))" << std::endl;
  } else {
    std::cout << ")" << std::endl;
  }
}

void StatementPrinter::operator()(ast::Return const& node) {
  std::cout << Indent(indentation) << "(Return";
  if (node.expression) {
    std::cout << std::endl;
    StatementPrinter print_expression(indentation + INDENT_STEP);
    print_expression(node.expression.get());
    std::cout << Indent(indentation) << ")" << std::endl;
  } else {
    std::cout << ")" << std::endl;
  }
}

void StatementPrinter::operator()(ast::With const& with) {
  int next = indentation + INDENT_STEP;
  StatementPrinter print_expression(next + INDENT_STEP);
  ASTPrinter print_statement(next + INDENT_STEP);

  std::cout << Indent(indentation) << "(With" << std::endl;

  std::cout << Indent(next) << "(context " << std::endl;
  print_expression(with.context);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(next) << "(body " << std::endl;
  print_statement(with.statement);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::LabelledStatement const& labelled) {
  int next = indentation + INDENT_STEP;
  ASTPrinter print_statement(next);

  std::cout << Indent(indentation) << "(LabelledStatement" << std::endl;
  std::cout << Indent(next) << "(label " << labelled.label << ")" << std::endl;
  print_statement(labelled.statement);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::Case const& clause) {
  int next = indentation + INDENT_STEP;
  StatementPrinter print_expression(next + INDENT_STEP);
  ASTPrinter print_statement(next);

  std::cout << Indent(indentation) << "(Case" << std::endl;

  std::cout << Indent(next) << "(match" << std::endl;
  print_expression(clause.match_clause);
  std::cout << Indent(next) << ")" << std::endl;

  print_statement(clause.statements);

  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::Switch const& conditional) {
  int next = indentation + INDENT_STEP;
  StatementPrinter print_case(next);
  StatementPrinter print_expression(next + INDENT_STEP);
  ASTPrinter print_statement(next + INDENT_STEP);

  std::cout << Indent(indentation) << "(Switch" << std::endl;

  std::cout << Indent(next) << "(condition" << std::endl;
  print_expression(conditional.condition);
  std::cout << Indent(next) << ")" << std::endl;

  for (std::vector<ast::Case>::const_iterator it = conditional.clauses.begin();
       it != conditional.clauses.end(); ++it) {
    print_case(*it);
  }

  if (conditional.default_clause) {
    std::cout << Indent(next) << "(Default" << std::endl;
    print_statement(conditional.default_clause.get());
    std::cout << Indent(next) << ")" << std::endl;
  }

  for (std::vector<ast::Case>::const_iterator it = conditional.other_clauses.begin();
       it != conditional.other_clauses.end(); ++it) {
    print_case(*it);
  }

  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::Throw const& node) {
  StatementPrinter print_expression(indentation + INDENT_STEP);
  std::cout << Indent(indentation) << "(Throw" << std::endl;
  print_expression(node.expression);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::Try const& node) {
  int next = indentation + INDENT_STEP;
  ASTPrinter print_statement(next);
  ASTPrinter print_nested_statement(next + INDENT_STEP);

  std::cout << Indent(indentation) << "(Try" << std::endl;
  print_statement(node.statements);

  if (node.catch_block) {
    ast::Catch block = node.catch_block.get();
    std::cout << Indent(next) << "(Catch" << std::endl;
    std::cout << Indent(next + INDENT_STEP) << "(exception_name " << block.exception_name << ")" << std::endl;
    print_nested_statement(block.statements);
    std::cout << Indent(next) << ")" << std::endl;
  }

  if (node.finally_block) {
    std::cout << Indent(next) << "(Finally" << std::endl;
    print_nested_statement(node.finally_block.get());
    std::cout << Indent(next) << ")" << std::endl;
  }

  std::cout << Indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(std::string const& debugger) {
  std::cout << Indent(indentation) << "(Debugger)" << std::endl;
}

void StatementPrinter::operator()(std::vector<ast::Statement> const& list) {
  ASTPrinter printer(indentation);
  printer(list);
}


ExpressionPrinter::ExpressionPrinter(int indent) : indentation(indent) {}

void ExpressionPrinter::operator()(ast::AssignmentExpression const& expression) {
  int next = indentation + INDENT_STEP;
  ExpressionPrinter print_lhs(next + INDENT_STEP);
  ExpressionPrinter print_conditional(next);

  std::cout << Indent(indentation) << "(AssignmentExpression" << std::endl;

  for (std::vector<ast::AssignmentOperation>::const_iterator it = expression.assignments.begin();
       it != expression.assignments.end(); ++it) {
    std::cout << Indent(next) << "(AssignmentOperation" << std::endl;
    print_lhs(it->lhs);
    std::cout << Indent(next + INDENT_STEP) << "(operator " << it->operator_ << ")" << std::endl;
    std::cout << Indent(next) << ")" << std::endl;
  }

  print_conditional(expression.rhs);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::ConditionalExpression const& expression) {
  int next = indentation + INDENT_STEP;
  ExpressionPrinter print_logical_or(next);
  ExpressionPrinter print_assignment(next + INDENT_STEP);

  std::cout << Indent(indentation) << "(ConditionalExpression" << std::endl;
  print_logical_or(expression.lhs);

  if (expression.conditional_clauses) {
    ast::ConditionalClauses ternary = expression.conditional_clauses.get();

    std::cout << Indent(next) << "(true_clause" << std::endl;
    print_assignment(ternary.true_clause.get());
    std::cout << Indent(next) << ")" << std::endl;

    std::cout << Indent(next) << "(false_clause" << std::endl;
    print_assignment(ternary.false_clause.get());
    std::cout << Indent(next) << ")" << std::endl;
  }

  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::LogicalOrExpression const& expression) {
  ExpressionPrinter print_logical_and(indentation + INDENT_STEP);

  std::cout << Indent(indentation) << "(LogicalOrExpression" << std::endl;
  print_logical_and(expression.lhs);
  std::for_each(expression.operations.begin(), expression.operations.end(), print_logical_and);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::LogicalAndExpression const& expression) {
  ExpressionPrinter print(indentation + INDENT_STEP);

  std::cout << Indent(indentation) << "(LogicalAndExpression" << std::endl;
  print(expression.lhs);
  std::for_each(expression.operations.begin(), expression.operations.end(), print);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::BitwiseOrExpression const& expression) {
  ExpressionPrinter print(indentation + INDENT_STEP);

  std::cout << Indent(indentation) << "(BitwiseOrExpression" << std::endl;
  print(expression.lhs);
  std::for_each(expression.operations.begin(), expression.operations.end(), print);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::BitwiseXorExpression const& expression) {
  ExpressionPrinter print(indentation + INDENT_STEP);

  std::cout << Indent(indentation) << "(BitwiseXorExpression" << std::endl;
  print(expression.lhs);
  std::for_each(expression.operations.begin(), expression.operations.end(), print);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::BitwiseAndExpression const& expression) {
  ExpressionPrinter print(indentation + INDENT_STEP);

  std::cout << Indent(indentation) << "(BitwiseAndExpression" << std::endl;
  print(expression.lhs);
  std::for_each(expression.operations.begin(), expression.operations.end(), print);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::EqualityExpression const& expression) {
  int next = indentation + INDENT_STEP;
  ExpressionPrinter print(next);

  std::cout << Indent(indentation) << "(EqualityExpression" << std::endl;
  print(expression.lhs);

  for (std::vector<ast::EqualityOperation>::const_iterator it = expression.operations.begin();
       it != expression.operations.end(); ++it) {
    std::cout << Indent(next) << "(operator " << it->operator_ << ")" << std::endl;
    print(it->rhs);
  }

  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::RelationalExpression const& expression) {
  int next = indentation + INDENT_STEP;
  ExpressionPrinter print(next);

  std::cout << Indent(indentation) << "(RelationalExpression" << std::endl;
  print(expression.lhs);

  for (std::vector<ast::RelationalOperation>::const_iterator it = expression.operations.begin();
       it != expression.operations.end(); ++it) {
    std::cout << Indent(next) << "(operator " << it->operator_ << ")" << std::endl;
    print(it->rhs);
  }

  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::ShiftExpression const& expression) {
  int next = indentation + INDENT_STEP;
  ExpressionPrinter print(next);

  std::cout << Indent(indentation) << "(ShiftExpression" << std::endl;
  print(expression.lhs);

  for (std::vector<ast::ShiftOperation>::const_iterator it = expression.operations.begin();
       it != expression.operations.end(); ++it) {
    std::cout << Indent(next) << "(operator " << it->operator_ << ")" << std::endl;
    print(it->rhs);
  }

  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::AdditiveExpression const& expression) {
  int next = indentation + INDENT_STEP;
  ExpressionPrinter print(next);

  std::cout << Indent(indentation) << "(AdditiveExpression" << std::endl;
  print(expression.lhs);

  for (std::vector<ast::AdditiveOperation>::const_iterator it = expression.operations.begin();
       it != expression.operations.end(); ++it) {
    std::cout << Indent(next) << "(operator " << it->operator_ << ")" << std::endl;
    print(it->rhs);
  }

  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::MultiplicativeExpression const& expression) {
  int next = indentation + INDENT_STEP;
  ExpressionPrinter print(next);

  std::cout << Indent(indentation) << "(MultiplicativeExpression" << std::endl;
  print(expression.lhs);

  for (std::vector<ast::MultiplicativeOperation>::const_iterator it = expression.operations.begin();
       it != expression.operations.end(); ++it) {
    std::cout << Indent(next) << "(operator " << it->operator_ << ")" << std::endl;
    print(it->rhs);
  }

  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::UnaryExpression const& expression) {
  int next = indentation + INDENT_STEP;
  ExpressionPrinter print(next);

  std::cout << Indent(indentation) << "(UnaryExpression" << std::endl;
  for (std::vector<std::string>::const_iterator it = expression.operators.begin();
       it != expression.operators.end(); ++it) {
    std::cout << Indent(next) << "(operator " << *it << ")" << std::endl;
  }

  print(expression.rhs);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::PostfixExpression const& expression) {
  int next = indentation + INDENT_STEP;
  ExpressionPrinter print(next);

  std::cout << Indent(indentation) << "(PostfixExpression" << std::endl;
  print(expression.lhs);

  if (expression.operator_) {
    std::cout << Indent(next) << "(operator " << expression.operator_.get() << ")" << std::endl;
  }

  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::LhsExpression const& expression) {
  ExpressionPrinter printer(indentation + INDENT_STEP);
  std::cout << Indent(indentation) << "(LhsExpression" << std::endl;
  boost::apply_visitor(printer, expression);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::CallExpression const& expression) {
  std::cout << Indent(indentation) << "(CallExpression)" << std::endl;
}

void ExpressionPrinter::operator()(ast::NewExpression const& expression) {
  int next = indentation + INDENT_STEP;
  ExpressionPrinter print(next);

  std::cout << Indent(indentation) << "(NewExpression" << std::endl;
  for (std::vector<std::string>::const_iterator it = expression.operators.begin();
       it != expression.operators.end(); ++it) {
    std::cout << Indent(next) << "(operator " << *it << ")" << std::endl;
  }

  print(expression.member);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::MemberExpression const& expression) {
  ExpressionPrinter printer(indentation + INDENT_STEP);
  std::cout << Indent(indentation) << "(MemberExpression" << std::endl;
  boost::apply_visitor(printer, expression);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::MemberAccess const& expression) {
  int next = indentation + INDENT_STEP;
  ExpressionPrinter print(next + INDENT_STEP);
  std::cout << Indent(indentation) << "(MemberAccess" << std::endl;

  std::cout << Indent(next) << "(member " << std::endl;
  print(expression.member);
  std::cout << Indent(next) << ")" << std::endl;

  if (!expression.modifiers.empty()) {
    std::cout << Indent(next) << "(modifiers " << std::endl;
    std::for_each(expression.modifiers.begin(), expression.modifiers.end(), print);
    std::cout << Indent(next) << ")" << std::endl;
  }

  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::MemberOptions const& expression) {
  ExpressionPrinter printer(indentation + INDENT_STEP);
  std::cout << Indent(indentation) << "(MemberOptions" << std::endl;
  boost::apply_visitor(printer, expression);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::MemberModifier const& expression) {
  std::cout << Indent(indentation) << "(MemberModifier)" << std::endl;
}

void ExpressionPrinter::operator()(ast::Instantiation const& expression) {
  int next = indentation + INDENT_STEP;
  ExpressionPrinter print(next);
  StatementPrinter print_statement(next + INDENT_STEP);

  std::cout << Indent(indentation) << "(Instantiation" << std::endl;
  print(expression.member);

  std::cout << Indent(next) << "(arguments " << std::endl;
  print_statement(expression.arguments);
  std::cout << Indent(next) << ")" << std::endl;

  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::PrimaryExpression const& expression) {
  PrimaryExpressionPrinter printer(indentation + INDENT_STEP);
  std::cout << Indent(indentation) << "(PrimaryExpression" << std::endl;
  boost::apply_visitor(printer, expression);
  std::cout << Indent(indentation) << ")" << std::endl;
}

void ExpressionPrinter::operator()(ast::FunctionExpression const& expression) {
  std::cout << Indent(indentation) << "(FunctionExpression)" << std::endl;
}


PrimaryExpressionPrinter::PrimaryExpressionPrinter(int indent) : indentation(indent) {}

void PrimaryExpressionPrinter::operator()(ast::This const& node) {
  std::cout << Indent(indentation) << "(This)" << std::endl;
}

void PrimaryExpressionPrinter::operator()(std::string const& identifier) {
  std::cout << Indent(indentation) << "(identifier " << identifier << ")" << std::endl;
}

void PrimaryExpressionPrinter::operator()(ast::Literal const& literal) {
  std::cout << Indent(indentation) << "(literal " << literal << ")" << std::endl;
}

void PrimaryExpressionPrinter::operator()(ast::Expression const& expression) {
  StatementPrinter print(indentation);
  print(expression);
}


} // namespace kunjs

namespace std {
std::ostream& operator<<(std::ostream& stream, kunjs::ast::Null const& value) {
  stream << "null";
  return stream;
}
}

