#include "kunjs/printer.h"
#include "kunjs/ast.h"

#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <iostream>
#include <string>

namespace kunjs {

namespace {

std::string indent(int level) {
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
  std::cout << indent(indentation) << "(Program " << std::endl;
  indentation += 2;
  std::for_each(program.begin(), program.end(), *this);
  indentation -= 2;
  std::cout << indent(indentation) << ")" << std::endl;
}

void ASTPrinter::operator()(ast::SourceElement const& element) const {
  std::cout << indent(indentation) << "(SourceElement " << std::endl;
  indentation += 2;
  boost::apply_visitor(*this, element);
  indentation -= 2;
  std::cout << indent(indentation) << ")" << std::endl;
}

void ASTPrinter::operator()(ast::FunctionDeclaration const& function) const {
  std::cout << indent(indentation) << "(FunctionDeclaration)" << std::endl;
}

void ASTPrinter::operator()(ast::Statement const& statement) const {
  std::cout << indent(indentation) << "(Statement " << std::endl;
  StatementPrinter printer(indentation + 2);
  boost::apply_visitor(printer, statement);
  std::cout << indent(indentation) << ")" << std::endl;
}


StatementPrinter::StatementPrinter(int indent) : indentation(indent) {}

void StatementPrinter::operator()(ast::Expression const& expression) {
  std::cout << indent(indentation) << "(Expression " << std::endl;
  ExpressionPrinter printer(indentation + 2);
  std::for_each(expression.begin(), expression.end(), printer);
  std::cout << indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::Var const& var) {
  int next_level = indentation + 2;
  ExpressionPrinter print_expression(next_level);
  for (ast::Var::const_iterator it = var.begin(); it != var.end(); ++it) {
    std::cout << indent(indentation) << "(Var" << std::endl;
    std::cout << indent(next_level) << "(name " << it->name << ")" << std::endl;
    if (it->assignment) print_expression(it->assignment.get());
    std::cout << indent(indentation) << ")" << std::endl;
  }
}

void StatementPrinter::operator()(ast::Noop const& noop) {
  std::cout << indent(indentation) << "(Empty)" << std::endl;
}

void StatementPrinter::operator()(ast::If const& conditional) {
  int next = indentation + 2;
  StatementPrinter print_expression(next + 2);
  ASTPrinter print_statement(next + 2);

  std::cout << indent(indentation) << "(If " << std::endl;

  std::cout << indent(next) << "(condition " << std::endl;
  print_expression(conditional.condition);
  std::cout << indent(next) << ")" << std::endl;

  std::cout << indent(next) << "(true_clause " << std::endl;
  print_statement(conditional.true_clause);
  std::cout << indent(next) << ")" << std::endl;

  if (conditional.false_clause) {
    std::cout << indent(next) << "(false_clause " << std::endl;
    print_statement(conditional.false_clause.get());
    std::cout << indent(next) << ")" << std::endl;
  }

  std::cout << indent(indentation) << ")" << std::endl;
}

void StatementPrinter::operator()(ast::DoWhile const& loop) {
  std::cout << indent(indentation) << "(DoWhile)" << std::endl;
}

void StatementPrinter::operator()(ast::While const& loop) {
  std::cout << indent(indentation) << "(While)" << std::endl;
}

void StatementPrinter::operator()(ast::For const& loop) {
  std::cout << indent(indentation) << "(For)" << std::endl;
}

void StatementPrinter::operator()(ast::ForWithVar const& loop) {
  std::cout << indent(indentation) << "(ForWithVar)" << std::endl;
}

void StatementPrinter::operator()(ast::Foreach const& loop) {
  std::cout << indent(indentation) << "(Foreach)" << std::endl;
}

void StatementPrinter::operator()(ast::ForeachWithVar const& loop) {
  std::cout << indent(indentation) << "(ForeachWithVar)" << std::endl;
}

void StatementPrinter::operator()(ast::Continue const& node) {
  std::cout << indent(indentation) << "(Continue)" << std::endl;
}

void StatementPrinter::operator()(ast::Break const& node) {
  std::cout << indent(indentation) << "(Break)" << std::endl;
}

void StatementPrinter::operator()(ast::Return const& node) {
  std::cout << indent(indentation) << "(Return)" << std::endl;
}

void StatementPrinter::operator()(ast::With const& with) {
  std::cout << indent(indentation) << "(With)" << std::endl;
}

void StatementPrinter::operator()(ast::LabelledStatement const& labeled) {
  std::cout << indent(indentation) << "(LabelledStatement)" << std::endl;
}

void StatementPrinter::operator()(ast::Switch const& conditional) {
  std::cout << indent(indentation) << "(Switch)" << std::endl;
}

void StatementPrinter::operator()(ast::Throw const& node) {
  std::cout << indent(indentation) << "(Throw)" << std::endl;
}

void StatementPrinter::operator()(ast::Try const& node) {
  std::cout << indent(indentation) << "(Try)" << std::endl;
}

void StatementPrinter::operator()(std::string const& debugger) {
  std::cout << indent(indentation) << "(Debugger)" << std::endl;
}

void StatementPrinter::operator()(std::vector<ast::Statement> const& list) {
  ASTPrinter printer(indentation);
  printer(list);
}


ExpressionPrinter::ExpressionPrinter(int indent) : indentation(indent) {}

void ExpressionPrinter::operator()(ast::AssignmentExpression const& expression) {
  std::cout << indent(indentation) << "(AssignmentExpression)" << std::endl;
}


} // namespace kunjs

