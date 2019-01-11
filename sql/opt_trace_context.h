#ifndef OPT_TRACE_CONTEXT_INCLUDED
#define OPT_TRACE_CONTEXT_INCLUDED

#include "sql_array.h"

class Opt_trace_stmt;

class Opt_trace_context
{
public:
   Opt_trace_context();
  ~Opt_trace_context();

void start(THD *thd, TABLE_LIST *tbl,
           enum enum_sql_command sql_command,
           const char *query,
           size_t query_length,
           const CHARSET_INFO *query_charset);
void end();
void set_query(const char *query, size_t length, const CHARSET_INFO *charset);
void flush_optimizer_trace();

private:

Opt_trace_stmt* top_trace()
{
  return *(traces->front());
}

public:

/*
  This returns the top trace from the list of traces. This function
  is used when we want to see the contents of the INFORMATION_SCHEMA.OPTIMIZER_TRACE
table.
*/

Opt_trace_stmt* get_top_trace()
{
  if (!traces || !traces->elements())
    return NULL;
  return top_trace();
}

/*
  This returns the current trace, to which we are still writing and has not been finished
*/

Opt_trace_stmt* get_current_trace()
{
  return current_trace;
}

Json_writer* get_current_json();

bool empty()
{
  return traces && (static_cast<uint>(traces->elements()) != 0);
}

bool is_started()
{
  return current_trace;
}

void missing_privilege();

static const char *flag_names[];
enum
{
  FLAG_DEFAULT = 0,
  FLAG_ENABLED = 1 << 0
};

private:
  /*
    List of traces (currently it stores only 1 trace)
  */
  Dynamic_array<Opt_trace_stmt*> *traces;
  Opt_trace_stmt *current_trace;
  /*
    TRUE: if we allocate memory for list of traces
    FALSE: otherwise
  */
  bool inited;
};
#endif
