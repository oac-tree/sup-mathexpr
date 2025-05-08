Welcome to sup-mathexpr's documentation!
==================================================

Overview
~~~~~~~~

This library provides an interface to expose the functionality of the `exprtk <https://www.partow.net/programming/exprtk/index.html>` mathematical expression parsing and evaluation library to CODAC's SUP.

Key Features
~~~~~~~~~~~~

+ Exposes ``exprtk``'s powerful mathematical expression parsing and evaluation capabilities.
+ Supports scalar and vector variables for flexible expression evaluation.
+ Handles variable assignments within expressions.
+ Provides robust error handling through custom exceptions.
+ The ``ExpressionContext`` class acts as the main entry point for evaluating expressions. It:
   - Collects variables from the expression.
   - Retrieves their values from the ``IVariableStore``.
   - Evaluates the expression using ``exprtk``.
   - Updates variables in the ``IVariableStore`` if assignments are present in the expression.

Example
~~~~~~~

.. code-block:: c++

  #include "expression_context.h"
  #include "my_variable_store.h"  // Your implementation of IVariableStore

  sup::mathexpr::MyVariableStore var_store;
  sup::mathexpr::ExpressionContext context(var_store);

  std::string expression = "x + y * 2";
  double result = context.EvaluateExpression(expression);
  std::cout << "Result: " << result << std::endl;

.. toctree::
   :maxdepth: 2
   :caption: Contents:

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
