Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
  8.08      0.13     0.13   183081     0.00     0.00  Txn::init()
  8.08      0.26     0.13   820367     0.00     0.00  SimpleData::store(Key, void*, Txn*)
  6.83      0.37     0.11  1552986     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::find(Key const&) const
  5.90      0.47     0.10   726189     0.00     0.00  LockManager::unlock(Key, Txn*)
  5.59      0.56     0.09  4003869     0.00     0.00  Lock::owners_unsafe(std::deque<Txn*, std::allocator<Txn*> >*)
  5.59      0.65     0.09  1566040     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::find(Key const&) const
  4.97      0.73     0.08   816888     0.00     0.00  Txn::write(Key, void*)
  4.35      0.80     0.07  2496446     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::find(Key const&) const
  4.35      0.87     0.07   803827     0.00     0.00  Txn::read(Key, void*)
  4.35      0.94     0.07   778094     0.00     0.00  LockManager::rlockedby(Key, Txn*)
  4.35      1.01     0.07   770266     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::equal_range(Key const&)
  3.73      1.07     0.06    80056     0.00     0.00  microtxn(Txn*)
  3.73      1.13     0.06   773629     0.00     0.00  LockManager::wlockedby(Key, Txn*)
  3.73      1.19     0.06   752513     0.00     0.00  SimpleData::read(Key, void*, Txn*)
  3.42      1.24     0.06  1709483     0.00     0.00  LockManager::wlock(Key, Txn*)
  3.11      1.29     0.05  3571274     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, void**> >, std::pair<Key const, void**> const&)
  1.86      1.32     0.03  3992375     0.00     0.00  std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_initialize_map(unsigned long)
  1.86      1.35     0.03   772776     0.00     0.00  DLock::wakeupowners()
  1.86      1.38     0.03   162695     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_erase(std::_Rb_tree_node<std::pair<Key const, void**> >*)
  1.24      1.40     0.02  5463219     0.00     0.00  Key::valsize() const
  1.24      1.42     0.02  4332740     0.00     0.00  std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_destroy_nodes(Txn***, Txn***)
  1.24      1.44     0.02   785049     0.00     0.00  std::deque<request, std::allocator<request> >::erase(std::_Deque_iterator<request, request&, request*>)
  1.24      1.46     0.02    90886     0.00     0.00  Txn::readphase()
  0.93      1.48     0.02  3360357     0.00     0.00  contains(std::deque<Txn*, std::allocator<Txn*> >*, Txn*)
  0.62      1.49     0.01  8797717     0.00     0.00  Key::Key(long)
  0.62      1.50     0.01  3785678     0.00     0.00  std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_create_nodes(Txn***, Txn***)
  0.62      1.51     0.01  3518963     0.00     0.00  Configuration::partitionContainingRecord(long) const
  0.62      1.52     0.01  3426420     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, void**> const&)
  0.62      1.53     0.01  3345332     0.00     0.00  contains(std::deque<int, std::allocator<int> >*, int)
  0.62      1.54     0.01  1722818     0.00     0.00  DLock::lock(Txn*, bool)
  0.62      1.55     0.01  1637427     0.00     0.00  Lock::owners(std::deque<Txn*, std::allocator<Txn*> >*)
  0.62      1.56     0.01   830640     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, void*> >, std::pair<Key const, void*> const&)
  0.62      1.57     0.01   826597     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::erase(Key const&)
  0.62      1.58     0.01   820968     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::erase(std::_Rb_tree_iterator<std::pair<Key const, void*> >, std::_Rb_tree_iterator<std::pair<Key const, void*> >)
  0.62      1.59     0.01   172361     0.00     0.00  Txn::Txn()
  0.31      1.59     0.01    81197     0.00     0.00  __gnu_cxx::hashtable<std::pair<long const, Txn*>, long, __gnu_cxx::hash<long>, std::_Select1st<std::pair<long const, Txn*> >, std::equal_to<long>, std::allocator<Txn*> >::erase(long const&)
  0.31      1.60     0.01     1252     0.00     0.00  std::deque<Txn*, std::allocator<Txn*> >::_M_pop_front_aux()
  0.31      1.60     0.01       80     0.06     0.06  DLock::DLock()
  0.31      1.61     0.01        1     5.00     5.00  std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, int> >, std::pair<int const, int> const&)
  0.31      1.61     0.01                             LockManager::print()
  0.00      1.61     0.00  4190935     0.00     0.00  std::_Deque_base<Txn*, std::allocator<Txn*> >::~_Deque_base()
  0.00      1.61     0.00  4170170     0.00     0.00  Lock::end()
  0.00      1.61     0.00  3873790     0.00     0.00  Lock::begin()
  0.00      1.61     0.00   783694     0.00     0.00  DLock::unlock(Txn*)
  0.00      1.61     0.00   749422     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, void*> const&)
  0.00      1.61     0.00   358831     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_unique(std::pair<Key const, void**> const&)
  0.00      1.61     0.00   349503     0.00     0.00  std::_Deque_base<int, std::allocator<int> >::_M_initialize_map(unsigned long)
  0.00      1.61     0.00   324373     0.00     0.00  std::_Deque_base<int, std::allocator<int> >::_M_create_nodes(int**, int**)
  0.00      1.61     0.00   261687     0.00     0.00  tm()
  0.00      1.61     0.00   174964     0.00     0.00  microtxninput(int)
  0.00      1.61     0.00   167812     0.00     0.00  std::_Deque_base<Message*, std::allocator<Message*> >::_M_create_nodes(Message***, Message***)
  0.00      1.61     0.00   162782     0.00     0.00  std::_Deque_base<int, std::allocator<int> >::_M_destroy_nodes(int**, int**)
  0.00      1.61     0.00   162685     0.00     0.00  std::_Deque_base<Message*, std::allocator<Message*> >::_M_initialize_map(unsigned long)
  0.00      1.61     0.00   158603     0.00     0.00  std::_Deque_base<int, std::allocator<int> >::~_Deque_base()
  0.00      1.61     0.00    89468     0.00     0.00  Txn::~Txn()
  0.00      1.61     0.00    86602     0.00     0.00  std::_Deque_base<Message*, std::allocator<Message*> >::_M_destroy_nodes(Message***, Message***)
  0.00      1.61     0.00    83174     0.00     0.00  std::_Deque_base<Message*, std::allocator<Message*> >::~_Deque_base()
  0.00      1.61     0.00    81110     0.00     0.00  Txn::begin(Worker*)
  0.00      1.61     0.00    79980     0.00     0.00  Txn::writephase()
  0.00      1.61     0.00    76893     0.00     0.00  Txn::messagephase()
  0.00      1.61     0.00    52556     0.00     0.00  std::deque<request, std::allocator<request> >::_M_push_back_aux(request const&)
  0.00      1.61     0.00    24617     0.00     0.00  std::deque<request, std::allocator<request> >::_M_pop_front_aux()
  0.00      1.61     0.00     2708     0.00     0.00  std::deque<Txn*, std::allocator<Txn*> >::_M_push_back_aux(Txn* const&)
  0.00      1.61     0.00      497     0.00     0.00  std::deque<request, std::allocator<request> >::_M_reallocate_map(unsigned long, bool)
  0.00      1.61     0.00       90     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, Lock*> >, std::pair<Key const, Lock*> const&)
  0.00      1.61     0.00       84     0.00     0.00  Lock::Lock()
  0.00      1.61     0.00       77     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, Lock*> const&)
  0.00      1.61     0.00       76     0.00     0.00  std::_Deque_base<request, std::allocator<request> >::_M_initialize_map(unsigned long)
  0.00      1.61     0.00       74     0.00     0.00  std::_Deque_base<request, std::allocator<request> >::_M_create_nodes(request**, request**)
  0.00      1.61     0.00       43     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::equal_range(Key const&)
  0.00      1.61     0.00       42     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::erase(Key const&)
  0.00      1.61     0.00       39     0.00     0.00  std::_Deque_base<request, std::allocator<request> >::~_Deque_base()
  0.00      1.61     0.00       36     0.00     0.00  std::_Deque_base<request, std::allocator<request> >::_M_destroy_nodes(request**, request**)
  0.00      1.61     0.00       36     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::erase(std::_Rb_tree_iterator<std::pair<Key const, Lock*> >, std::_Rb_tree_iterator<std::pair<Key const, Lock*> >)
  0.00      1.61     0.00       13     0.00     0.00  std::deque<Txn*, std::allocator<Txn*> >::_M_reallocate_map(unsigned long, bool)
  0.00      1.61     0.00        5     0.00     0.00  Configuration::ProcessConfigLine(char*, char*)
  0.00      1.61     0.00        4     0.00     0.00  std::vector<DBNode*, std::allocator<DBNode*> >::_M_insert_aux(__gnu_cxx::__normal_iterator<DBNode**, std::vector<DBNode*, std::allocator<DBNode*> > >, DBNode* const&)
  0.00      1.61     0.00        4     0.00     0.00  std::_Rb_tree<int, std::pair<int const, Node*>, std::_Select1st<std::pair<int const, Node*> >, std::less<int>, std::allocator<std::pair<int const, Node*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, Node*> const&)
  0.00      1.61     0.00        4     0.00     0.00  std::_Rb_tree<int, std::pair<int const, Node*>, std::_Select1st<std::pair<int const, Node*> >, std::less<int>, std::allocator<std::pair<int const, Node*> > >::_M_insert_unique(std::pair<int const, Node*> const&)
  0.00      1.61     0.00        3     0.00     0.00  std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&)
  0.00      1.61     0.00        3     0.00     0.00  std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&)
  0.00      1.61     0.00        2     0.00     0.00  prepare_serversock(int)
  0.00      1.61     0.00        2     0.00     0.00  std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> >::_M_insert_aux(__gnu_cxx::__normal_iterator<PreprocessorNode**, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, PreprocessorNode* const&)
  0.00      1.61     0.00        2     0.00     0.00  std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&)
  0.00      1.61     0.00        2     0.00     0.00  std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_unique(std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&)
  0.00      1.61     0.00        2     0.00     0.00  std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&)
  0.00      1.61     0.00        2     0.00     0.00  std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_unique(std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&)
  0.00      1.61     0.00        1     0.00     0.00  global constructors keyed to _Z8containsPSt5dequeIP3TxnSaIS1_EES1_
  0.00      1.61     0.00        1     0.00     0.00  global constructors keyed to _Z8containsPSt5dequeIiSaIiEEi
  0.00      1.61     0.00        1     0.00     0.00  global constructors keyed to _ZN10SimpleDataC2Ev
  0.00      1.61     0.00        1     0.00     0.00  global constructors keyed to _ZN11CoordinatorC2Ev
  0.00      1.61     0.00        1     0.00     0.00  global constructors keyed to _ZN16RemoteConnection10_instancesE
  0.00      1.61     0.00        1     0.00     0.00  global constructors keyed to tablewidth
  0.00      1.61     0.00        1     0.00   456.72  localcoordinator()
  0.00      1.61     0.00        1     0.00     0.00  ginits(int, char*)
  0.00      1.61     0.00        1     0.00     5.04  microinit()
  0.00      1.61     0.00        1     0.00     0.00  SimpleData::SimpleData()
  0.00      1.61     0.00        1     0.00     0.00  Configuration::ReadFromFile(std::string const&)
  0.00      1.61     0.00        1     0.00     0.00  Configuration::Configuration(int, std::string const&)
  0.00      1.61     0.00        1     0.00     0.00  RemoteConnection::GetInstance(Configuration const&)
  0.00      1.61     0.00        1     0.00     0.00  RemoteConnection::InitializeDB(DBNode const*)
  0.00      1.61     0.00        1     0.00     0.00  RemoteConnection::TryReconnect()
  0.00      1.61     0.00        1     0.00     0.00  RemoteConnection::RemoteConnection(Configuration const&)
  0.00      1.61     0.00        1     0.00    40.00  Worker::mainloop()
  0.00      1.61     0.00        1     0.00     0.00  Worker::Worker(int)
  0.00      1.61     0.00        1     0.00     0.00  __gnu_cxx::hashtable<std::pair<long const, Txn*>, long, __gnu_cxx::hash<long>, std::_Select1st<std::pair<long const, Txn*> >, std::equal_to<long>, std::allocator<Txn*> >::clear()
  0.00      1.61     0.00        1     0.00     0.00  std::_Rb_tree<int, std::pair<int const, Node*>, std::_Select1st<std::pair<int const, Node*> >, std::less<int>, std::allocator<std::pair<int const, Node*> > >::find(int const&) const
  0.00      1.61     0.00        1     0.00     0.00  std::vector<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*, std::allocator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*> >::_M_fill_insert(__gnu_cxx::__normal_iterator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >**, std::vector<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*, std::allocator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*> > >, unsigned long, __gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >* const&)
  0.00      1.61     0.00        1     0.00     0.00  std::vector<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*, std::allocator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*> >::reserve(unsigned long)
  0.00      1.61     0.00        1     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_unique(std::pair<Key const, Lock*> const&)
  0.00      1.61     0.00        1     0.00     0.00  std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_unique(std::pair<Key const, void*> const&)
  0.00      1.61     0.00        1     0.00     0.00  std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Configuration const* const, RemoteConnection*> const&)
  0.00      1.61     0.00        1     0.00     0.00  std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::_M_insert_unique(std::pair<Configuration const* const, RemoteConnection*> const&)
  0.00      1.61     0.00        1     0.00     0.00  std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::find(Configuration const* const&)
  0.00      1.61     0.00        1     0.00     0.00  std::_Rb_tree<std::string, std::pair<std::string const, std::string>, std::_Select1st<std::pair<std::string const, std::string> >, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<std::string const, std::string> const&)
  0.00      1.61     0.00        1     0.00     0.00  std::_Rb_tree<std::string, std::pair<std::string const, std::string>, std::_Select1st<std::pair<std::string const, std::string> >, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >::_M_insert_unique(std::pair<std::string const, std::string> const&)
  0.00      1.61     0.00        1     0.00     0.00  std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, MediatorNode*> const&)
  0.00      1.61     0.00        1     0.00     0.00  std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_unique(std::pair<int const, MediatorNode*> const&)
  0.00      1.61     0.00        1     0.00     0.00  std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, MediatorNode*> >, std::pair<int const, MediatorNode*> const&)
  0.00      1.61     0.00        1     0.00     0.00  std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, int> const&)
  0.00      1.61     0.00        1     0.00     0.00  std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_unique(std::pair<int const, int> const&)

 %         the percentage of the total running time of the
time       program used by this function.

cumulative a running sum of the number of seconds accounted
 seconds   for by this function and those listed above it.

 self      the number of seconds accounted for by this
seconds    function alone.  This is the major sort for this
           listing.

calls      the number of times this function was invoked, if
           this function is profiled, else blank.
 
 self      the average number of milliseconds spent in this
ms/call    function per call, if this function is profiled,
	   else blank.

 total     the average number of milliseconds spent in this
ms/call    function and its descendents per call, if this 
	   function is profiled, else blank.

name       the name of the function.  This is the minor sort
           for this listing. The index shows the location of
	   the function in the gprof listing. If the index is
	   in parenthesis it shows where it would appear in
	   the gprof listing if it were to be printed.

		     Call graph (explanation follows)


granularity: each sample hit covers 2 byte(s) for 0.62% of 1.61 seconds

index % time    self  children    called     name
                                                 <spontaneous>
[1]     57.7    0.00    0.93                 runtxn(Txn*) [1]
                0.00    0.39   79980/79980       Txn::writephase() [6]
                0.02    0.25   90886/90886       Txn::readphase() [8]
                0.06    0.21   80056/80056       microtxn(Txn*) [9]
                0.00    0.00   76893/76893       Txn::messagephase() [54]
-----------------------------------------------
                                                 <spontaneous>
[2]     28.7    0.00    0.46                 main [2]
                0.00    0.46       1/1           localcoordinator() [4]
                0.00    0.01       1/1           microinit() [47]
                0.00    0.00       1/1           ginits(int, char*) [106]
                0.00    0.00       1/1           Worker::Worker(int) [114]
-----------------------------------------------
                0.00    0.46  174964/174964      localcoordinator() [4]
[3]     28.4    0.00    0.46  174964         microtxninput(int) [3]
                0.13    0.32  183081/183081      Txn::init() [5]
                0.01    0.00  172361/172361      Txn::Txn() [46]
                0.00    0.00    2708/2708        std::deque<Txn*, std::allocator<Txn*> >::_M_push_back_aux(Txn* const&) [76]
-----------------------------------------------
                0.00    0.46       1/1           main [2]
[4]     28.4    0.00    0.46       1         localcoordinator() [4]
                0.00    0.46  174964/174964      microtxninput(int) [3]
                0.00    0.00  173091/261687      tm() [66]
-----------------------------------------------
                0.13    0.32  183081/183081      microtxninput(int) [3]
[5]     27.7    0.13    0.32  183081         Txn::init() [5]
                0.06    0.17 1709483/1709483     LockManager::wlock(Key, Txn*) [10]
                0.05    0.01 3571274/3571274     std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, void**> >, std::pair<Key const, void**> const&) [26]
                0.01    0.00 3518963/3518963     Configuration::partitionContainingRecord(long) const [41]
                0.01    0.00 3176219/3345332     contains(std::deque<int, std::allocator<int> >*, int) [43]
                0.01    0.00 7210538/8797717     Key::Key(long) [39]
-----------------------------------------------
                0.00    0.39   79980/79980       runtxn(Txn*) [1]
[6]     24.1    0.00    0.39   79980         Txn::writephase() [6]
                0.13    0.16  820267/820367      SimpleData::store(Key, void*, Txn*) [7]
                0.06    0.04  773629/773629      LockManager::wlockedby(Key, Txn*) [17]
-----------------------------------------------
                0.00    0.00     100/820367      microinit() [47]
                0.13    0.16  820267/820367      Txn::writephase() [6]
[7]     18.0    0.13    0.16  820367         SimpleData::store(Key, void*, Txn*) [7]
                0.01    0.08  826597/826597      std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::erase(Key const&) [21]
                0.05    0.00  772303/1552986     std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::find(Key const&) const [15]
                0.01    0.00  830640/830640      std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, void*> >, std::pair<Key const, void*> const&) [44]
                0.01    0.00 1547178/5463219     Key::valsize() const [34]
-----------------------------------------------
                0.02    0.25   90886/90886       runtxn(Txn*) [1]
[8]     16.9    0.02    0.25   90886         Txn::readphase() [8]
                0.06    0.06  752513/752513      SimpleData::read(Key, void*, Txn*) [13]
                0.07    0.04  778094/778094      LockManager::rlockedby(Key, Txn*) [16]
                0.02    0.00  770230/2496446     std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::find(Key const&) const [24]
                0.00    0.00  723095/5463219     Key::valsize() const [34]
-----------------------------------------------
                0.06    0.21   80056/80056       runtxn(Txn*) [1]
[9]     16.7    0.06    0.21   80056         microtxn(Txn*) [9]
                0.08    0.03  816888/816888      Txn::write(Key, void*) [14]
                0.07    0.03  803827/803827      Txn::read(Key, void*) [18]
                0.00    0.00 1587079/8797717     Key::Key(long) [39]
-----------------------------------------------
                0.06    0.17 1709483/1709483     Txn::init() [5]
[10]    14.2    0.06    0.17 1709483         LockManager::wlock(Key, Txn*) [10]
                0.09    0.00 1566040/1566040     std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::find(Key const&) const [20]
                0.01    0.07 1722818/1722818     DLock::lock(Txn*, bool) [22]
                0.01    0.00      80/80          DLock::DLock() [50]
                0.00    0.00      90/90          std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, Lock*> >, std::pair<Key const, Lock*> const&) [78]
-----------------------------------------------
                0.10    0.08  726189/726189      Txn::finish() [12]
[11]    10.8    0.10    0.08  726189         LockManager::unlock(Key, Txn*) [11]
                0.00    0.08  783694/783694      DLock::unlock(Txn*) [23]
                0.00    0.00      42/42          std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::erase(Key const&) [84]
                0.00    0.00      39/39          std::_Deque_base<request, std::allocator<request> >::~_Deque_base() [85]
-----------------------------------------------
                                                 <spontaneous>
[12]    10.8    0.00    0.17                 Txn::finish() [12]
                0.10    0.08  726189/726189      LockManager::unlock(Key, Txn*) [11]
-----------------------------------------------
                0.06    0.06  752513/752513      Txn::readphase() [8]
[13]     7.3    0.06    0.06  752513         SimpleData::read(Key, void*, Txn*) [13]
                0.06    0.00  780683/1552986     std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::find(Key const&) const [15]
                0.00    0.00  791067/5463219     Key::valsize() const [34]
-----------------------------------------------
                0.08    0.03  816888/816888      microtxn(Txn*) [9]
[14]     6.8    0.08    0.03  816888         Txn::write(Key, void*) [14]
                0.02    0.00  867923/2496446     std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::find(Key const&) const [24]
                0.01    0.00 1587963/5463219     Key::valsize() const [34]
-----------------------------------------------
                0.05    0.00  772303/1552986     SimpleData::store(Key, void*, Txn*) [7]
                0.06    0.00  780683/1552986     SimpleData::read(Key, void*, Txn*) [13]
[15]     6.8    0.11    0.00 1552986         std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::find(Key const&) const [15]
-----------------------------------------------
                0.07    0.04  778094/778094      Txn::readphase() [8]
[16]     6.8    0.07    0.04  778094         LockManager::rlockedby(Key, Txn*) [16]
                0.00    0.02  818370/1637427     Lock::owners(std::deque<Txn*, std::allocator<Txn*> >*) [28]
                0.01    0.00  808147/3992375     std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_initialize_map(unsigned long) [29]
                0.00    0.00  871678/4190935     std::_Deque_base<Txn*, std::allocator<Txn*> >::~_Deque_base() [36]
                0.00    0.00  800712/3360357     contains(std::deque<Txn*, std::allocator<Txn*> >*, Txn*) [38]
-----------------------------------------------
                0.06    0.04  773629/773629      Txn::writephase() [6]
[17]     6.1    0.06    0.04  773629         LockManager::wlockedby(Key, Txn*) [17]
                0.01    0.02  819057/1637427     Lock::owners(std::deque<Txn*, std::allocator<Txn*> >*) [28]
                0.01    0.00  750552/3992375     std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_initialize_map(unsigned long) [29]
                0.00    0.00  818727/4190935     std::_Deque_base<Txn*, std::allocator<Txn*> >::~_Deque_base() [36]
                0.00    0.00  795567/3360357     contains(std::deque<Txn*, std::allocator<Txn*> >*, Txn*) [38]
-----------------------------------------------
                0.07    0.03  803827/803827      microtxn(Txn*) [9]
[18]     6.0    0.07    0.03  803827         Txn::read(Key, void*) [18]
                0.02    0.00  858293/2496446     std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::find(Key const&) const [24]
                0.00    0.00  813916/5463219     Key::valsize() const [34]
-----------------------------------------------
                0.02    0.00  753746/4003869     DLock::wakeupowners() [27]
                0.04    0.00 1623450/4003869     DLock::lock(Txn*, bool) [22]
                0.04    0.00 1626673/4003869     Lock::owners(std::deque<Txn*, std::allocator<Txn*> >*) [28]
[19]     5.6    0.09    0.00 4003869         Lock::owners_unsafe(std::deque<Txn*, std::allocator<Txn*> >*) [19]
-----------------------------------------------
                0.09    0.00 1566040/1566040     LockManager::wlock(Key, Txn*) [10]
[20]     5.6    0.09    0.00 1566040         std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::find(Key const&) const [20]
-----------------------------------------------
                0.01    0.08  826597/826597      SimpleData::store(Key, void*, Txn*) [7]
[21]     5.6    0.01    0.08  826597         std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::erase(Key const&) [21]
                0.07    0.00  770266/770266      std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::equal_range(Key const&) [25]
                0.01    0.00  820968/820968      std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::erase(std::_Rb_tree_iterator<std::pair<Key const, void*> >, std::_Rb_tree_iterator<std::pair<Key const, void*> >) [45]
-----------------------------------------------
                0.01    0.07 1722818/1722818     LockManager::wlock(Key, Txn*) [10]
[22]     4.9    0.01    0.07 1722818         DLock::lock(Txn*, bool) [22]
                0.04    0.00 1623450/4003869     Lock::owners_unsafe(std::deque<Txn*, std::allocator<Txn*> >*) [19]
                0.01    0.00 1622957/3992375     std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_initialize_map(unsigned long) [29]
                0.00    0.01 1749136/4190935     std::_Deque_base<Txn*, std::allocator<Txn*> >::~_Deque_base() [36]
                0.01    0.00 1764078/3360357     contains(std::deque<Txn*, std::allocator<Txn*> >*, Txn*) [38]
                0.00    0.00 1757760/4170170     Lock::end() [61]
                0.00    0.00 1569779/3873790     Lock::begin() [62]
                0.00    0.00   52556/52556       std::deque<request, std::allocator<request> >::_M_push_back_aux(request const&) [74]
-----------------------------------------------
                0.00    0.08  783694/783694      LockManager::unlock(Key, Txn*) [11]
[23]     4.9    0.00    0.08  783694         DLock::unlock(Txn*) [23]
                0.03    0.03  772776/772776      DLock::wakeupowners() [27]
                0.02    0.00  785049/785049      std::deque<request, std::allocator<request> >::erase(std::_Deque_iterator<request, request&, request*>) [37]
                0.00    0.00  767402/3873790     Lock::begin() [62]
                0.00    0.00  719579/4170170     Lock::end() [61]
-----------------------------------------------
                0.02    0.00  770230/2496446     Txn::readphase() [8]
                0.02    0.00  858293/2496446     Txn::read(Key, void*) [18]
                0.02    0.00  867923/2496446     Txn::write(Key, void*) [14]
[24]     4.3    0.07    0.00 2496446         std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::find(Key const&) const [24]
-----------------------------------------------
                0.07    0.00  770266/770266      std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::erase(Key const&) [21]
[25]     4.3    0.07    0.00  770266         std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::equal_range(Key const&) [25]
-----------------------------------------------
                0.05    0.01 3571274/3571274     Txn::init() [5]
[26]     3.7    0.05    0.01 3571274         std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, void**> >, std::pair<Key const, void**> const&) [26]
                0.01    0.00 3082955/3426420     std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, void**> const&) [42]
                0.00    0.00  358831/358831      std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_unique(std::pair<Key const, void**> const&) [53]
-----------------------------------------------
                0.03    0.03  772776/772776      DLock::unlock(Txn*) [23]
[27]     3.6    0.03    0.03  772776         DLock::wakeupowners() [27]
                0.02    0.00  753746/4003869     Lock::owners_unsafe(std::deque<Txn*, std::allocator<Txn*> >*) [19]
                0.01    0.00  810716/3992375     std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_initialize_map(unsigned long) [29]
                0.00    0.00  751394/4190935     std::_Deque_base<Txn*, std::allocator<Txn*> >::~_Deque_base() [36]
-----------------------------------------------
                0.00    0.02  818370/1637427     LockManager::rlockedby(Key, Txn*) [16]
                0.01    0.02  819057/1637427     LockManager::wlockedby(Key, Txn*) [17]
[28]     2.9    0.01    0.04 1637427         Lock::owners(std::deque<Txn*, std::allocator<Txn*> >*) [28]
                0.04    0.00 1626673/4003869     Lock::owners_unsafe(std::deque<Txn*, std::allocator<Txn*> >*) [19]
                0.00    0.00 1692831/4170170     Lock::end() [61]
                0.00    0.00 1536609/3873790     Lock::begin() [62]
-----------------------------------------------
                0.00    0.00       3/3992375     global constructors keyed to tablewidth [55]
                0.01    0.00  750552/3992375     LockManager::wlockedby(Key, Txn*) [17]
                0.01    0.00  808147/3992375     LockManager::rlockedby(Key, Txn*) [16]
                0.01    0.00  810716/3992375     DLock::wakeupowners() [27]
                0.01    0.00 1622957/3992375     DLock::lock(Txn*, bool) [22]
[29]     2.5    0.03    0.01 3992375         std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_initialize_map(unsigned long) [29]
                0.01    0.00 3785678/3785678     std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_create_nodes(Txn***, Txn***) [40]
-----------------------------------------------
                0.00    0.04       1/1           workermainloop(void*) [31]
[30]     2.5    0.00    0.04       1         Worker::mainloop() [30]
                0.00    0.03   89468/89468       Txn::~Txn() [33]
                0.01    0.00   81197/81197       __gnu_cxx::hashtable<std::pair<long const, Txn*>, long, __gnu_cxx::hash<long>, std::_Select1st<std::pair<long const, Txn*> >, std::equal_to<long>, std::allocator<Txn*> >::erase(long const&) [48]
                0.01    0.00    1252/1252        std::deque<Txn*, std::allocator<Txn*> >::_M_pop_front_aux() [49]
                0.00    0.00   83174/83174       std::_Deque_base<Message*, std::allocator<Message*> >::~_Deque_base() [72]
                0.00    0.00   81110/81110       Txn::begin(Worker*) [73]
-----------------------------------------------
                                                 <spontaneous>
[31]     2.5    0.00    0.04                 workermainloop(void*) [31]
                0.00    0.04       1/1           Worker::mainloop() [30]
-----------------------------------------------
                             1630059             std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_erase(std::_Rb_tree_node<std::pair<Key const, void**> >*) [32]
                0.03    0.00  162695/162695      Txn::~Txn() [33]
[32]     1.9    0.03    0.00  162695+1630059 std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_erase(std::_Rb_tree_node<std::pair<Key const, void**> >*) [32]
                             1630059             std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_erase(std::_Rb_tree_node<std::pair<Key const, void**> >*) [32]
-----------------------------------------------
                0.00    0.03   89468/89468       Worker::mainloop() [30]
[33]     1.9    0.00    0.03   89468         Txn::~Txn() [33]
                0.03    0.00  162695/162695      std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_erase(std::_Rb_tree_node<std::pair<Key const, void**> >*) [32]
                0.00    0.00  158603/158603      std::_Deque_base<int, std::allocator<int> >::~_Deque_base() [70]
-----------------------------------------------
                0.00    0.00  723095/5463219     Txn::readphase() [8]
                0.00    0.00  791067/5463219     SimpleData::read(Key, void*, Txn*) [13]
                0.00    0.00  813916/5463219     Txn::read(Key, void*) [18]
                0.01    0.00 1547178/5463219     SimpleData::store(Key, void*, Txn*) [7]
                0.01    0.00 1587963/5463219     Txn::write(Key, void*) [14]
[34]     1.2    0.02    0.00 5463219         Key::valsize() const [34]
-----------------------------------------------
                0.02    0.00 4332740/4332740     std::_Deque_base<Txn*, std::allocator<Txn*> >::~_Deque_base() [36]
[35]     1.2    0.02    0.00 4332740         std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_destroy_nodes(Txn***, Txn***) [35]
-----------------------------------------------
                0.00    0.00  751394/4190935     DLock::wakeupowners() [27]
                0.00    0.00  818727/4190935     LockManager::wlockedby(Key, Txn*) [17]
                0.00    0.00  871678/4190935     LockManager::rlockedby(Key, Txn*) [16]
                0.00    0.01 1749136/4190935     DLock::lock(Txn*, bool) [22]
[36]     1.2    0.00    0.02 4190935         std::_Deque_base<Txn*, std::allocator<Txn*> >::~_Deque_base() [36]
                0.02    0.00 4332740/4332740     std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_destroy_nodes(Txn***, Txn***) [35]
-----------------------------------------------
                0.02    0.00  785049/785049      DLock::unlock(Txn*) [23]
[37]     1.2    0.02    0.00  785049         std::deque<request, std::allocator<request> >::erase(std::_Deque_iterator<request, request&, request*>) [37]
                0.00    0.00   24617/24617       std::deque<request, std::allocator<request> >::_M_pop_front_aux() [75]
-----------------------------------------------
                0.00    0.00  795567/3360357     LockManager::wlockedby(Key, Txn*) [17]
                0.00    0.00  800712/3360357     LockManager::rlockedby(Key, Txn*) [16]
                0.01    0.00 1764078/3360357     DLock::lock(Txn*, bool) [22]
[38]     0.9    0.02    0.00 3360357         contains(std::deque<Txn*, std::allocator<Txn*> >*, Txn*) [38]
-----------------------------------------------
                0.00    0.00     100/8797717     microinit() [47]
                0.00    0.00 1587079/8797717     microtxn(Txn*) [9]
                0.01    0.00 7210538/8797717     Txn::init() [5]
[39]     0.6    0.01    0.00 8797717         Key::Key(long) [39]
-----------------------------------------------
                0.01    0.00 3785678/3785678     std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_initialize_map(unsigned long) [29]
[40]     0.6    0.01    0.00 3785678         std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_create_nodes(Txn***, Txn***) [40]
-----------------------------------------------
                0.01    0.00 3518963/3518963     Txn::init() [5]
[41]     0.6    0.01    0.00 3518963         Configuration::partitionContainingRecord(long) const [41]
-----------------------------------------------
                0.00    0.00  343465/3426420     std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_unique(std::pair<Key const, void**> const&) [53]
                0.01    0.00 3082955/3426420     std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, void**> >, std::pair<Key const, void**> const&) [26]
[42]     0.6    0.01    0.00 3426420         std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, void**> const&) [42]
-----------------------------------------------
                0.00    0.00  169113/3345332     Txn::messagephase() [54]
                0.01    0.00 3176219/3345332     Txn::init() [5]
[43]     0.6    0.01    0.00 3345332         contains(std::deque<int, std::allocator<int> >*, int) [43]
-----------------------------------------------
                0.01    0.00  830640/830640      SimpleData::store(Key, void*, Txn*) [7]
[44]     0.6    0.01    0.00  830640         std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, void*> >, std::pair<Key const, void*> const&) [44]
                0.00    0.00  749421/749422      std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, void*> const&) [63]
                0.00    0.00       1/1           std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_unique(std::pair<Key const, void*> const&) [120]
-----------------------------------------------
                0.01    0.00  820968/820968      std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::erase(Key const&) [21]
[45]     0.6    0.01    0.00  820968         std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::erase(std::_Rb_tree_iterator<std::pair<Key const, void*> >, std::_Rb_tree_iterator<std::pair<Key const, void*> >) [45]
-----------------------------------------------
                0.01    0.00  172361/172361      microtxninput(int) [3]
[46]     0.6    0.01    0.00  172361         Txn::Txn() [46]
                0.00    0.00  349503/349503      std::_Deque_base<int, std::allocator<int> >::_M_initialize_map(unsigned long) [64]
                0.00    0.00  162685/162685      std::_Deque_base<Message*, std::allocator<Message*> >::_M_initialize_map(unsigned long) [69]
-----------------------------------------------
                0.00    0.01       1/1           main [2]
[47]     0.3    0.00    0.01       1         microinit() [47]
                0.01    0.00       1/1           std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, int> >, std::pair<int const, int> const&) [51]
                0.00    0.00     100/820367      SimpleData::store(Key, void*, Txn*) [7]
                0.00    0.00     100/8797717     Key::Key(long) [39]
-----------------------------------------------
                0.01    0.00   81197/81197       Worker::mainloop() [30]
[48]     0.3    0.01    0.00   81197         __gnu_cxx::hashtable<std::pair<long const, Txn*>, long, __gnu_cxx::hash<long>, std::_Select1st<std::pair<long const, Txn*> >, std::equal_to<long>, std::allocator<Txn*> >::erase(long const&) [48]
-----------------------------------------------
                0.01    0.00    1252/1252        Worker::mainloop() [30]
[49]     0.3    0.01    0.00    1252         std::deque<Txn*, std::allocator<Txn*> >::_M_pop_front_aux() [49]
-----------------------------------------------
                0.01    0.00      80/80          LockManager::wlock(Key, Txn*) [10]
[50]     0.3    0.01    0.00      80         DLock::DLock() [50]
                0.00    0.00      84/84          Lock::Lock() [79]
-----------------------------------------------
                0.01    0.00       1/1           microinit() [47]
[51]     0.3    0.01    0.00       1         std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, int> >, std::pair<int const, int> const&) [51]
                0.00    0.00       1/1           std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_unique(std::pair<int const, int> const&) [130]
-----------------------------------------------
                                                 <spontaneous>
[52]     0.3    0.01    0.00                 LockManager::print() [52]
-----------------------------------------------
                0.00    0.00  358831/358831      std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, void**> >, std::pair<Key const, void**> const&) [26]
[53]     0.1    0.00    0.00  358831         std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_unique(std::pair<Key const, void**> const&) [53]
                0.00    0.00  343465/3426420     std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, void**> const&) [42]
-----------------------------------------------
                0.00    0.00   76893/76893       runtxn(Txn*) [1]
[54]     0.0    0.00    0.00   76893         Txn::messagephase() [54]
                0.00    0.00  169113/3345332     contains(std::deque<int, std::allocator<int> >*, int) [43]
-----------------------------------------------
                0.00    0.00       1/1           __do_global_ctors_aux [56]
[55]     0.0    0.00    0.00       1         global constructors keyed to tablewidth [55]
                0.00    0.00       3/3992375     std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_initialize_map(unsigned long) [29]
                0.00    0.00       1/1           std::vector<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*, std::allocator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*> >::reserve(unsigned long) [118]
                0.00    0.00       1/1           std::vector<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*, std::allocator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*> >::_M_fill_insert(__gnu_cxx::__normal_iterator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >**, std::vector<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*, std::allocator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*> > >, unsigned long, __gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >* const&) [117]
-----------------------------------------------
                                                 <spontaneous>
[56]     0.0    0.00    0.00                 __do_global_ctors_aux [56]
                0.00    0.00       1/1           global constructors keyed to tablewidth [55]
                0.00    0.00       1/1           global constructors keyed to _ZN11CoordinatorC2Ev [104]
                0.00    0.00       1/1           global constructors keyed to _Z8containsPSt5dequeIP3TxnSaIS1_EES1_ [101]
                0.00    0.00       1/1           global constructors keyed to _Z8containsPSt5dequeIiSaIiEEi [102]
                0.00    0.00       1/1           global constructors keyed to _ZN10SimpleDataC2Ev [103]
                0.00    0.00       1/1           global constructors keyed to _ZN16RemoteConnection10_instancesE [105]
-----------------------------------------------
                0.00    0.00  719579/4170170     DLock::unlock(Txn*) [23]
                0.00    0.00 1692831/4170170     Lock::owners(std::deque<Txn*, std::allocator<Txn*> >*) [28]
                0.00    0.00 1757760/4170170     DLock::lock(Txn*, bool) [22]
[61]     0.0    0.00    0.00 4170170         Lock::end() [61]
-----------------------------------------------
                0.00    0.00  767402/3873790     DLock::unlock(Txn*) [23]
                0.00    0.00 1536609/3873790     Lock::owners(std::deque<Txn*, std::allocator<Txn*> >*) [28]
                0.00    0.00 1569779/3873790     DLock::lock(Txn*, bool) [22]
[62]     0.0    0.00    0.00 3873790         Lock::begin() [62]
-----------------------------------------------
                0.00    0.00       1/749422      std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_unique(std::pair<Key const, void*> const&) [120]
                0.00    0.00  749421/749422      std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, void*> >, std::pair<Key const, void*> const&) [44]
[63]     0.0    0.00    0.00  749422         std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, void*> const&) [63]
-----------------------------------------------
                0.00    0.00  349503/349503      Txn::Txn() [46]
[64]     0.0    0.00    0.00  349503         std::_Deque_base<int, std::allocator<int> >::_M_initialize_map(unsigned long) [64]
                0.00    0.00  324373/324373      std::_Deque_base<int, std::allocator<int> >::_M_create_nodes(int**, int**) [65]
-----------------------------------------------
                0.00    0.00  324373/324373      std::_Deque_base<int, std::allocator<int> >::_M_initialize_map(unsigned long) [64]
[65]     0.0    0.00    0.00  324373         std::_Deque_base<int, std::allocator<int> >::_M_create_nodes(int**, int**) [65]
-----------------------------------------------
                0.00    0.00       1/261687      ginits(int, char*) [106]
                0.00    0.00   88595/261687      Txn::begin(Worker*) [73]
                0.00    0.00  173091/261687      localcoordinator() [4]
[66]     0.0    0.00    0.00  261687         tm() [66]
-----------------------------------------------
                0.00    0.00  167812/167812      std::_Deque_base<Message*, std::allocator<Message*> >::_M_initialize_map(unsigned long) [69]
[67]     0.0    0.00    0.00  167812         std::_Deque_base<Message*, std::allocator<Message*> >::_M_create_nodes(Message***, Message***) [67]
-----------------------------------------------
                0.00    0.00  162782/162782      std::_Deque_base<int, std::allocator<int> >::~_Deque_base() [70]
[68]     0.0    0.00    0.00  162782         std::_Deque_base<int, std::allocator<int> >::_M_destroy_nodes(int**, int**) [68]
-----------------------------------------------
                0.00    0.00  162685/162685      Txn::Txn() [46]
[69]     0.0    0.00    0.00  162685         std::_Deque_base<Message*, std::allocator<Message*> >::_M_initialize_map(unsigned long) [69]
                0.00    0.00  167812/167812      std::_Deque_base<Message*, std::allocator<Message*> >::_M_create_nodes(Message***, Message***) [67]
-----------------------------------------------
                0.00    0.00  158603/158603      Txn::~Txn() [33]
[70]     0.0    0.00    0.00  158603         std::_Deque_base<int, std::allocator<int> >::~_Deque_base() [70]
                0.00    0.00  162782/162782      std::_Deque_base<int, std::allocator<int> >::_M_destroy_nodes(int**, int**) [68]
-----------------------------------------------
                0.00    0.00   86602/86602       std::_Deque_base<Message*, std::allocator<Message*> >::~_Deque_base() [72]
[71]     0.0    0.00    0.00   86602         std::_Deque_base<Message*, std::allocator<Message*> >::_M_destroy_nodes(Message***, Message***) [71]
-----------------------------------------------
                0.00    0.00   83174/83174       Worker::mainloop() [30]
[72]     0.0    0.00    0.00   83174         std::_Deque_base<Message*, std::allocator<Message*> >::~_Deque_base() [72]
                0.00    0.00   86602/86602       std::_Deque_base<Message*, std::allocator<Message*> >::_M_destroy_nodes(Message***, Message***) [71]
-----------------------------------------------
                0.00    0.00   81110/81110       Worker::mainloop() [30]
[73]     0.0    0.00    0.00   81110         Txn::begin(Worker*) [73]
                0.00    0.00   88595/261687      tm() [66]
-----------------------------------------------
                0.00    0.00   52556/52556       DLock::lock(Txn*, bool) [22]
[74]     0.0    0.00    0.00   52556         std::deque<request, std::allocator<request> >::_M_push_back_aux(request const&) [74]
                0.00    0.00     497/497         std::deque<request, std::allocator<request> >::_M_reallocate_map(unsigned long, bool) [77]
-----------------------------------------------
                0.00    0.00   24617/24617       std::deque<request, std::allocator<request> >::erase(std::_Deque_iterator<request, request&, request*>) [37]
[75]     0.0    0.00    0.00   24617         std::deque<request, std::allocator<request> >::_M_pop_front_aux() [75]
-----------------------------------------------
                0.00    0.00    2708/2708        microtxninput(int) [3]
[76]     0.0    0.00    0.00    2708         std::deque<Txn*, std::allocator<Txn*> >::_M_push_back_aux(Txn* const&) [76]
                0.00    0.00      13/13          std::deque<Txn*, std::allocator<Txn*> >::_M_reallocate_map(unsigned long, bool) [88]
-----------------------------------------------
                0.00    0.00     497/497         std::deque<request, std::allocator<request> >::_M_push_back_aux(request const&) [74]
[77]     0.0    0.00    0.00     497         std::deque<request, std::allocator<request> >::_M_reallocate_map(unsigned long, bool) [77]
-----------------------------------------------
                0.00    0.00      90/90          LockManager::wlock(Key, Txn*) [10]
[78]     0.0    0.00    0.00      90         std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, Lock*> >, std::pair<Key const, Lock*> const&) [78]
                0.00    0.00      76/77          std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, Lock*> const&) [80]
                0.00    0.00       1/1           std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_unique(std::pair<Key const, Lock*> const&) [119]
-----------------------------------------------
                0.00    0.00      84/84          DLock::DLock() [50]
[79]     0.0    0.00    0.00      84         Lock::Lock() [79]
                0.00    0.00      76/76          std::_Deque_base<request, std::allocator<request> >::_M_initialize_map(unsigned long) [81]
-----------------------------------------------
                0.00    0.00       1/77          std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_unique(std::pair<Key const, Lock*> const&) [119]
                0.00    0.00      76/77          std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, Lock*> >, std::pair<Key const, Lock*> const&) [78]
[80]     0.0    0.00    0.00      77         std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, Lock*> const&) [80]
-----------------------------------------------
                0.00    0.00      76/76          Lock::Lock() [79]
[81]     0.0    0.00    0.00      76         std::_Deque_base<request, std::allocator<request> >::_M_initialize_map(unsigned long) [81]
                0.00    0.00      74/74          std::_Deque_base<request, std::allocator<request> >::_M_create_nodes(request**, request**) [82]
-----------------------------------------------
                0.00    0.00      74/74          std::_Deque_base<request, std::allocator<request> >::_M_initialize_map(unsigned long) [81]
[82]     0.0    0.00    0.00      74         std::_Deque_base<request, std::allocator<request> >::_M_create_nodes(request**, request**) [82]
-----------------------------------------------
                0.00    0.00      43/43          std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::erase(Key const&) [84]
[83]     0.0    0.00    0.00      43         std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::equal_range(Key const&) [83]
-----------------------------------------------
                0.00    0.00      42/42          LockManager::unlock(Key, Txn*) [11]
[84]     0.0    0.00    0.00      42         std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::erase(Key const&) [84]
                0.00    0.00      43/43          std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::equal_range(Key const&) [83]
                0.00    0.00      36/36          std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::erase(std::_Rb_tree_iterator<std::pair<Key const, Lock*> >, std::_Rb_tree_iterator<std::pair<Key const, Lock*> >) [87]
-----------------------------------------------
                0.00    0.00      39/39          LockManager::unlock(Key, Txn*) [11]
[85]     0.0    0.00    0.00      39         std::_Deque_base<request, std::allocator<request> >::~_Deque_base() [85]
                0.00    0.00      36/36          std::_Deque_base<request, std::allocator<request> >::_M_destroy_nodes(request**, request**) [86]
-----------------------------------------------
                0.00    0.00      36/36          std::_Deque_base<request, std::allocator<request> >::~_Deque_base() [85]
[86]     0.0    0.00    0.00      36         std::_Deque_base<request, std::allocator<request> >::_M_destroy_nodes(request**, request**) [86]
-----------------------------------------------
                0.00    0.00      36/36          std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::erase(Key const&) [84]
[87]     0.0    0.00    0.00      36         std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::erase(std::_Rb_tree_iterator<std::pair<Key const, Lock*> >, std::_Rb_tree_iterator<std::pair<Key const, Lock*> >) [87]
-----------------------------------------------
                0.00    0.00      13/13          std::deque<Txn*, std::allocator<Txn*> >::_M_push_back_aux(Txn* const&) [76]
[88]     0.0    0.00    0.00      13         std::deque<Txn*, std::allocator<Txn*> >::_M_reallocate_map(unsigned long, bool) [88]
-----------------------------------------------
                0.00    0.00       5/5           Configuration::ReadFromFile(std::string const&) [108]
[89]     0.0    0.00    0.00       5         Configuration::ProcessConfigLine(char*, char*) [89]
                0.00    0.00       4/4           std::_Rb_tree<int, std::pair<int const, Node*>, std::_Select1st<std::pair<int const, Node*> >, std::less<int>, std::allocator<std::pair<int const, Node*> > >::_M_insert_unique(std::pair<int const, Node*> const&) [92]
                0.00    0.00       4/4           std::vector<DBNode*, std::allocator<DBNode*> >::_M_insert_aux(__gnu_cxx::__normal_iterator<DBNode**, std::vector<DBNode*, std::allocator<DBNode*> > >, DBNode* const&) [90]
                0.00    0.00       3/3           std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&) [94]
                0.00    0.00       2/2           std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&) [99]
                0.00    0.00       2/2           std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> >::_M_insert_aux(__gnu_cxx::__normal_iterator<PreprocessorNode**, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, PreprocessorNode* const&) [96]
                0.00    0.00       1/1           std::_Rb_tree<std::string, std::pair<std::string const, std::string>, std::_Select1st<std::pair<std::string const, std::string> >, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >::_M_insert_unique(std::pair<std::string const, std::string> const&) [125]
                0.00    0.00       1/1           std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, MediatorNode*> >, std::pair<int const, MediatorNode*> const&) [128]
-----------------------------------------------
                0.00    0.00       4/4           Configuration::ProcessConfigLine(char*, char*) [89]
[90]     0.0    0.00    0.00       4         std::vector<DBNode*, std::allocator<DBNode*> >::_M_insert_aux(__gnu_cxx::__normal_iterator<DBNode**, std::vector<DBNode*, std::allocator<DBNode*> > >, DBNode* const&) [90]
-----------------------------------------------
                0.00    0.00       4/4           std::_Rb_tree<int, std::pair<int const, Node*>, std::_Select1st<std::pair<int const, Node*> >, std::less<int>, std::allocator<std::pair<int const, Node*> > >::_M_insert_unique(std::pair<int const, Node*> const&) [92]
[91]     0.0    0.00    0.00       4         std::_Rb_tree<int, std::pair<int const, Node*>, std::_Select1st<std::pair<int const, Node*> >, std::less<int>, std::allocator<std::pair<int const, Node*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, Node*> const&) [91]
-----------------------------------------------
                0.00    0.00       4/4           Configuration::ProcessConfigLine(char*, char*) [89]
[92]     0.0    0.00    0.00       4         std::_Rb_tree<int, std::pair<int const, Node*>, std::_Select1st<std::pair<int const, Node*> >, std::less<int>, std::allocator<std::pair<int const, Node*> > >::_M_insert_unique(std::pair<int const, Node*> const&) [92]
                0.00    0.00       4/4           std::_Rb_tree<int, std::pair<int const, Node*>, std::_Select1st<std::pair<int const, Node*> >, std::less<int>, std::allocator<std::pair<int const, Node*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, Node*> const&) [91]
-----------------------------------------------
                0.00    0.00       1/3           std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&) [94]
                0.00    0.00       2/3           std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_unique(std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&) [100]
[93]     0.0    0.00    0.00       3         std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&) [93]
-----------------------------------------------
                0.00    0.00       3/3           Configuration::ProcessConfigLine(char*, char*) [89]
[94]     0.0    0.00    0.00       3         std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&) [94]
                0.00    0.00       2/2           std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_unique(std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&) [100]
                0.00    0.00       1/3           std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&) [93]
-----------------------------------------------
                0.00    0.00       2/2           RemoteConnection::InitializeDB(DBNode const*) [111]
[95]     0.0    0.00    0.00       2         prepare_serversock(int) [95]
-----------------------------------------------
                0.00    0.00       2/2           Configuration::ProcessConfigLine(char*, char*) [89]
[96]     0.0    0.00    0.00       2         std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> >::_M_insert_aux(__gnu_cxx::__normal_iterator<PreprocessorNode**, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, PreprocessorNode* const&) [96]
-----------------------------------------------
                0.00    0.00       2/2           std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_unique(std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&) [98]
[97]     0.0    0.00    0.00       2         std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&) [97]
-----------------------------------------------
                0.00    0.00       2/2           std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&) [99]
[98]     0.0    0.00    0.00       2         std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_unique(std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&) [98]
                0.00    0.00       2/2           std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&) [97]
-----------------------------------------------
                0.00    0.00       2/2           Configuration::ProcessConfigLine(char*, char*) [89]
[99]     0.0    0.00    0.00       2         std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&) [99]
                0.00    0.00       2/2           std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_unique(std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&) [98]
-----------------------------------------------
                0.00    0.00       2/2           std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&) [94]
[100]    0.0    0.00    0.00       2         std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_unique(std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&) [100]
                0.00    0.00       2/3           std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&) [93]
-----------------------------------------------
                0.00    0.00       1/1           __do_global_ctors_aux [56]
[101]    0.0    0.00    0.00       1         global constructors keyed to _Z8containsPSt5dequeIP3TxnSaIS1_EES1_ [101]
-----------------------------------------------
                0.00    0.00       1/1           __do_global_ctors_aux [56]
[102]    0.0    0.00    0.00       1         global constructors keyed to _Z8containsPSt5dequeIiSaIiEEi [102]
-----------------------------------------------
                0.00    0.00       1/1           __do_global_ctors_aux [56]
[103]    0.0    0.00    0.00       1         global constructors keyed to _ZN10SimpleDataC2Ev [103]
-----------------------------------------------
                0.00    0.00       1/1           __do_global_ctors_aux [56]
[104]    0.0    0.00    0.00       1         global constructors keyed to _ZN11CoordinatorC2Ev [104]
-----------------------------------------------
                0.00    0.00       1/1           __do_global_ctors_aux [56]
[105]    0.0    0.00    0.00       1         global constructors keyed to _ZN16RemoteConnection10_instancesE [105]
-----------------------------------------------
                0.00    0.00       1/1           main [2]
[106]    0.0    0.00    0.00       1         ginits(int, char*) [106]
                0.00    0.00       1/1           SimpleData::SimpleData() [107]
                0.00    0.00       1/1           Configuration::Configuration(int, std::string const&) [109]
                0.00    0.00       1/1           RemoteConnection::GetInstance(Configuration const&) [110]
                0.00    0.00       1/261687      tm() [66]
-----------------------------------------------
                0.00    0.00       1/1           ginits(int, char*) [106]
[107]    0.0    0.00    0.00       1         SimpleData::SimpleData() [107]
-----------------------------------------------
                0.00    0.00       1/1           Configuration::Configuration(int, std::string const&) [109]
[108]    0.0    0.00    0.00       1         Configuration::ReadFromFile(std::string const&) [108]
                0.00    0.00       5/5           Configuration::ProcessConfigLine(char*, char*) [89]
-----------------------------------------------
                0.00    0.00       1/1           ginits(int, char*) [106]
[109]    0.0    0.00    0.00       1         Configuration::Configuration(int, std::string const&) [109]
                0.00    0.00       1/1           Configuration::ReadFromFile(std::string const&) [108]
-----------------------------------------------
                0.00    0.00       1/1           ginits(int, char*) [106]
[110]    0.0    0.00    0.00       1         RemoteConnection::GetInstance(Configuration const&) [110]
                0.00    0.00       1/1           std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::find(Configuration const* const&) [123]
                0.00    0.00       1/1           RemoteConnection::RemoteConnection(Configuration const&) [113]
                0.00    0.00       1/1           std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::_M_insert_unique(std::pair<Configuration const* const, RemoteConnection*> const&) [122]
-----------------------------------------------
                0.00    0.00       1/1           RemoteConnection::RemoteConnection(Configuration const&) [113]
[111]    0.0    0.00    0.00       1         RemoteConnection::InitializeDB(DBNode const*) [111]
                0.00    0.00       2/2           prepare_serversock(int) [95]
-----------------------------------------------
                0.00    0.00       1/1           RemoteConnection::RemoteConnection(Configuration const&) [113]
[112]    0.0    0.00    0.00       1         RemoteConnection::TryReconnect() [112]
-----------------------------------------------
                0.00    0.00       1/1           RemoteConnection::GetInstance(Configuration const&) [110]
[113]    0.0    0.00    0.00       1         RemoteConnection::RemoteConnection(Configuration const&) [113]
                0.00    0.00       1/1           std::_Rb_tree<int, std::pair<int const, Node*>, std::_Select1st<std::pair<int const, Node*> >, std::less<int>, std::allocator<std::pair<int const, Node*> > >::find(int const&) const [116]
                0.00    0.00       1/1           RemoteConnection::TryReconnect() [112]
                0.00    0.00       1/1           RemoteConnection::InitializeDB(DBNode const*) [111]
-----------------------------------------------
                0.00    0.00       1/1           main [2]
[114]    0.0    0.00    0.00       1         Worker::Worker(int) [114]
-----------------------------------------------
                0.00    0.00       1/1           __gnu_cxx::hash_map<long, Txn*, __gnu_cxx::hash<long>, std::equal_to<long>, std::allocator<Txn*> >::~hash_map() [184]
[115]    0.0    0.00    0.00       1         __gnu_cxx::hashtable<std::pair<long const, Txn*>, long, __gnu_cxx::hash<long>, std::_Select1st<std::pair<long const, Txn*> >, std::equal_to<long>, std::allocator<Txn*> >::clear() [115]
-----------------------------------------------
                0.00    0.00       1/1           RemoteConnection::RemoteConnection(Configuration const&) [113]
[116]    0.0    0.00    0.00       1         std::_Rb_tree<int, std::pair<int const, Node*>, std::_Select1st<std::pair<int const, Node*> >, std::less<int>, std::allocator<std::pair<int const, Node*> > >::find(int const&) const [116]
-----------------------------------------------
                0.00    0.00       1/1           global constructors keyed to tablewidth [55]
[117]    0.0    0.00    0.00       1         std::vector<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*, std::allocator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*> >::_M_fill_insert(__gnu_cxx::__normal_iterator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >**, std::vector<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*, std::allocator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*> > >, unsigned long, __gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >* const&) [117]
-----------------------------------------------
                0.00    0.00       1/1           global constructors keyed to tablewidth [55]
[118]    0.0    0.00    0.00       1         std::vector<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*, std::allocator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*> >::reserve(unsigned long) [118]
-----------------------------------------------
                0.00    0.00       1/1           std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, Lock*> >, std::pair<Key const, Lock*> const&) [78]
[119]    0.0    0.00    0.00       1         std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_unique(std::pair<Key const, Lock*> const&) [119]
                0.00    0.00       1/77          std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, Lock*> const&) [80]
-----------------------------------------------
                0.00    0.00       1/1           std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, void*> >, std::pair<Key const, void*> const&) [44]
[120]    0.0    0.00    0.00       1         std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_unique(std::pair<Key const, void*> const&) [120]
                0.00    0.00       1/749422      std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, void*> const&) [63]
-----------------------------------------------
                0.00    0.00       1/1           std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::_M_insert_unique(std::pair<Configuration const* const, RemoteConnection*> const&) [122]
[121]    0.0    0.00    0.00       1         std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Configuration const* const, RemoteConnection*> const&) [121]
-----------------------------------------------
                0.00    0.00       1/1           RemoteConnection::GetInstance(Configuration const&) [110]
[122]    0.0    0.00    0.00       1         std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::_M_insert_unique(std::pair<Configuration const* const, RemoteConnection*> const&) [122]
                0.00    0.00       1/1           std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Configuration const* const, RemoteConnection*> const&) [121]
-----------------------------------------------
                0.00    0.00       1/1           RemoteConnection::GetInstance(Configuration const&) [110]
[123]    0.0    0.00    0.00       1         std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::find(Configuration const* const&) [123]
-----------------------------------------------
                0.00    0.00       1/1           std::_Rb_tree<std::string, std::pair<std::string const, std::string>, std::_Select1st<std::pair<std::string const, std::string> >, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >::_M_insert_unique(std::pair<std::string const, std::string> const&) [125]
[124]    0.0    0.00    0.00       1         std::_Rb_tree<std::string, std::pair<std::string const, std::string>, std::_Select1st<std::pair<std::string const, std::string> >, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<std::string const, std::string> const&) [124]
-----------------------------------------------
                0.00    0.00       1/1           Configuration::ProcessConfigLine(char*, char*) [89]
[125]    0.0    0.00    0.00       1         std::_Rb_tree<std::string, std::pair<std::string const, std::string>, std::_Select1st<std::pair<std::string const, std::string> >, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >::_M_insert_unique(std::pair<std::string const, std::string> const&) [125]
                0.00    0.00       1/1           std::_Rb_tree<std::string, std::pair<std::string const, std::string>, std::_Select1st<std::pair<std::string const, std::string> >, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<std::string const, std::string> const&) [124]
-----------------------------------------------
                0.00    0.00       1/1           std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_unique(std::pair<int const, MediatorNode*> const&) [127]
[126]    0.0    0.00    0.00       1         std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, MediatorNode*> const&) [126]
-----------------------------------------------
                0.00    0.00       1/1           std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, MediatorNode*> >, std::pair<int const, MediatorNode*> const&) [128]
[127]    0.0    0.00    0.00       1         std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_unique(std::pair<int const, MediatorNode*> const&) [127]
                0.00    0.00       1/1           std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, MediatorNode*> const&) [126]
-----------------------------------------------
                0.00    0.00       1/1           Configuration::ProcessConfigLine(char*, char*) [89]
[128]    0.0    0.00    0.00       1         std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, MediatorNode*> >, std::pair<int const, MediatorNode*> const&) [128]
                0.00    0.00       1/1           std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_unique(std::pair<int const, MediatorNode*> const&) [127]
-----------------------------------------------
                0.00    0.00       1/1           std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_unique(std::pair<int const, int> const&) [130]
[129]    0.0    0.00    0.00       1         std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, int> const&) [129]
-----------------------------------------------
                0.00    0.00       1/1           std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, int> >, std::pair<int const, int> const&) [51]
[130]    0.0    0.00    0.00       1         std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_unique(std::pair<int const, int> const&) [130]
                0.00    0.00       1/1           std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, int> const&) [129]
-----------------------------------------------
                                   1             std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::_M_erase(std::_Rb_tree_node<std::pair<Configuration const* const, RemoteConnection*> >*) [225]
[225]    0.0    0.00    0.00       0+1       std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::_M_erase(std::_Rb_tree_node<std::pair<Configuration const* const, RemoteConnection*> >*) [225]
                                   1             std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::_M_erase(std::_Rb_tree_node<std::pair<Configuration const* const, RemoteConnection*> >*) [225]
-----------------------------------------------
                                   1             std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_erase(std::_Rb_tree_node<std::pair<int const, int> >*) [246]
[246]    0.0    0.00    0.00       0+1       std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_erase(std::_Rb_tree_node<std::pair<int const, int> >*) [246]
                                   1             std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_erase(std::_Rb_tree_node<std::pair<int const, int> >*) [246]
-----------------------------------------------

 This table describes the call tree of the program, and was sorted by
 the total amount of time spent in each function and its children.

 Each entry in this table consists of several lines.  The line with the
 index number at the left hand margin lists the current function.
 The lines above it list the functions that called this function,
 and the lines below it list the functions this one called.
 This line lists:
     index	A unique number given to each element of the table.
		Index numbers are sorted numerically.
		The index number is printed next to every function name so
		it is easier to look up where the function in the table.

     % time	This is the percentage of the `total' time that was spent
		in this function and its children.  Note that due to
		different viewpoints, functions excluded by options, etc,
		these numbers will NOT add up to 100%.

     self	This is the total amount of time spent in this function.

     children	This is the total amount of time propagated into this
		function by its children.

     called	This is the number of times the function was called.
		If the function called itself recursively, the number
		only includes non-recursive calls, and is followed by
		a `+' and the number of recursive calls.

     name	The name of the current function.  The index number is
		printed after it.  If the function is a member of a
		cycle, the cycle number is printed between the
		function's name and the index number.


 For the function's parents, the fields have the following meanings:

     self	This is the amount of time that was propagated directly
		from the function into this parent.

     children	This is the amount of time that was propagated from
		the function's children into this parent.

     called	This is the number of times this parent called the
		function `/' the total number of times the function
		was called.  Recursive calls to the function are not
		included in the number after the `/'.

     name	This is the name of the parent.  The parent's index
		number is printed after it.  If the parent is a
		member of a cycle, the cycle number is printed between
		the name and the index number.

 If the parents of the function cannot be determined, the word
 `<spontaneous>' is printed in the `name' field, and all the other
 fields are blank.

 For the function's children, the fields have the following meanings:

     self	This is the amount of time that was propagated directly
		from the child into the function.

     children	This is the amount of time that was propagated from the
		child's children to the function.

     called	This is the number of times the function called
		this child `/' the total number of times the child
		was called.  Recursive calls by the child are not
		listed in the number after the `/'.

     name	This is the name of the child.  The child's index
		number is printed after it.  If the child is a
		member of a cycle, the cycle number is printed
		between the name and the index number.

 If there are any cycles (circles) in the call graph, there is an
 entry for the cycle-as-a-whole.  This entry shows who called the
 cycle (as parents) and the members of the cycle (as children.)
 The `+' recursive calls entry shows the number of function calls that
 were internal to the cycle, and the calls entry for each member shows,
 for that member, how many times it was called from other members of
 the cycle.


Index by function name

 [101] global constructors keyed to _Z8containsPSt5dequeIP3TxnSaIS1_EES1_ (locks.cc) [61] Lock::end() [96] std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> >::_M_insert_aux(__gnu_cxx::__normal_iterator<PreprocessorNode**, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, PreprocessorNode* const&)
 [102] global constructors keyed to _Z8containsPSt5dequeIiSaIiEEi (txn.cc) [62] Lock::begin() [90] std::vector<DBNode*, std::allocator<DBNode*> >::_M_insert_aux(__gnu_cxx::__normal_iterator<DBNode**, std::vector<DBNode*, std::allocator<DBNode*> > >, DBNode* const&)
 [103] global constructors keyed to _ZN10SimpleDataC2Ev (data.cc) [28] Lock::owners(std::deque<Txn*, std::allocator<Txn*> >*) [117] std::vector<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*, std::allocator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*> >::_M_fill_insert(__gnu_cxx::__normal_iterator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >**, std::vector<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*, std::allocator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*> > >, unsigned long, __gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >* const&)
 [104] global constructors keyed to _ZN11CoordinatorC2Ev (threads.cc) [79] Lock::Lock() [118] std::vector<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*, std::allocator<__gnu_cxx::_Hashtable_node<std::pair<long const, Txn*> >*> >::reserve(unsigned long)
 [105] global constructors keyed to _ZN16RemoteConnection10_instancesE (remote.cc) [27] DLock::wakeupowners() [80] std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, Lock*> const&)
  [55] global constructors keyed to tablewidth (main.cc) [22] DLock::lock(Txn*, bool) [83] std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::equal_range(Key const&)
   [3] microtxninput(int)     [23] DLock::unlock(Txn*)   [119] std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_unique(std::pair<Key const, Lock*> const&)
   [4] localcoordinator()     [50] DLock::DLock()         [78] std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, Lock*> >, std::pair<Key const, Lock*> const&)
  [95] prepare_serversock(int) [30] Worker::mainloop()    [84] std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::erase(Key const&)
  [66] tm()                  [114] Worker::Worker(int)    [87] std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::erase(std::_Rb_tree_iterator<std::pair<Key const, Lock*> >, std::_Rb_tree_iterator<std::pair<Key const, Lock*> >)
 [106] ginits(int, char*)    [115] __gnu_cxx::hashtable<std::pair<long const, Txn*>, long, __gnu_cxx::hash<long>, std::_Select1st<std::pair<long const, Txn*> >, std::equal_to<long>, std::allocator<Txn*> >::clear() [42] std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, void**> const&)
  [38] contains(std::deque<Txn*, std::allocator<Txn*> >*, Txn*) [48] __gnu_cxx::hashtable<std::pair<long const, Txn*>, long, __gnu_cxx::hash<long>, std::_Select1st<std::pair<long const, Txn*> >, std::equal_to<long>, std::allocator<Txn*> >::erase(long const&) [53] std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_unique(std::pair<Key const, void**> const&)
  [43] contains(std::deque<int, std::allocator<int> >*, int) [41] Configuration::partitionContainingRecord(long) const [26] std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, void**> >, std::pair<Key const, void**> const&)
   [9] microtxn(Txn*)         [34] Key::valsize() const   [32] std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::_M_erase(std::_Rb_tree_node<std::pair<Key const, void**> >*)
  [47] microinit()            [20] std::_Rb_tree<Key, std::pair<Key const, Lock*>, std::_Select1st<std::pair<Key const, Lock*> >, KeyComp, std::allocator<std::pair<Key const, Lock*> > >::find(Key const&) const [63] std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Key const, void*> const&)
  [13] SimpleData::read(Key, void*, Txn*) [24] std::_Rb_tree<Key, std::pair<Key const, void**>, std::_Select1st<std::pair<Key const, void**> >, KeyComp, std::allocator<std::pair<Key const, void**> > >::find(Key const&) const [25] std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::equal_range(Key const&)
   [7] SimpleData::store(Key, void*, Txn*) [15] std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::find(Key const&) const [120] std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_unique(std::pair<Key const, void*> const&)
 [107] SimpleData::SimpleData() [116] std::_Rb_tree<int, std::pair<int const, Node*>, std::_Select1st<std::pair<int const, Node*> >, std::less<int>, std::allocator<std::pair<int const, Node*> > >::find(int const&) const [44] std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<Key const, void*> >, std::pair<Key const, void*> const&)
  [52] LockManager::print()   [82] std::_Deque_base<request, std::allocator<request> >::_M_create_nodes(request**, request**) [21] std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::erase(Key const&)
  [10] LockManager::wlock(Key, Txn*) [86] std::_Deque_base<request, std::allocator<request> >::_M_destroy_nodes(request**, request**) [45] std::_Rb_tree<Key, std::pair<Key const, void*>, std::_Select1st<std::pair<Key const, void*> >, KeyComp, std::allocator<std::pair<Key const, void*> > >::erase(std::_Rb_tree_iterator<std::pair<Key const, void*> >, std::_Rb_tree_iterator<std::pair<Key const, void*> >)
  [11] LockManager::unlock(Key, Txn*) [81] std::_Deque_base<request, std::allocator<request> >::_M_initialize_map(unsigned long) [121] std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<Configuration const* const, RemoteConnection*> const&)
  [16] LockManager::rlockedby(Key, Txn*) [85] std::_Deque_base<request, std::allocator<request> >::~_Deque_base() [122] std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::_M_insert_unique(std::pair<Configuration const* const, RemoteConnection*> const&)
  [17] LockManager::wlockedby(Key, Txn*) [40] std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_create_nodes(Txn***, Txn***) [123] std::_Rb_tree<Configuration const*, std::pair<Configuration const* const, RemoteConnection*>, std::_Select1st<std::pair<Configuration const* const, RemoteConnection*> >, std::less<Configuration const*>, std::allocator<std::pair<Configuration const* const, RemoteConnection*> > >::find(Configuration const* const&)
 [108] Configuration::ReadFromFile(std::string const&) [35] std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_destroy_nodes(Txn***, Txn***) [124] std::_Rb_tree<std::string, std::pair<std::string const, std::string>, std::_Select1st<std::pair<std::string const, std::string> >, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<std::string const, std::string> const&)
  [89] Configuration::ProcessConfigLine(char*, char*) [29] std::_Deque_base<Txn*, std::allocator<Txn*> >::_M_initialize_map(unsigned long) [125] std::_Rb_tree<std::string, std::pair<std::string const, std::string>, std::_Select1st<std::pair<std::string const, std::string> >, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >::_M_insert_unique(std::pair<std::string const, std::string> const&)
 [109] Configuration::Configuration(int, std::string const&) [36] std::_Deque_base<Txn*, std::allocator<Txn*> >::~_Deque_base() [126] std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, MediatorNode*> const&)
 [110] RemoteConnection::GetInstance(Configuration const&) [67] std::_Deque_base<Message*, std::allocator<Message*> >::_M_create_nodes(Message***, Message***) [127] std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_unique(std::pair<int const, MediatorNode*> const&)
 [111] RemoteConnection::InitializeDB(DBNode const*) [71] std::_Deque_base<Message*, std::allocator<Message*> >::_M_destroy_nodes(Message***, Message***) [128] std::_Rb_tree<int, std::pair<int const, MediatorNode*>, std::_Select1st<std::pair<int const, MediatorNode*> >, std::less<int>, std::allocator<std::pair<int const, MediatorNode*> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, MediatorNode*> >, std::pair<int const, MediatorNode*> const&)
 [112] RemoteConnection::TryReconnect() [69] std::_Deque_base<Message*, std::allocator<Message*> >::_M_initialize_map(unsigned long) [91] std::_Rb_tree<int, std::pair<int const, Node*>, std::_Select1st<std::pair<int const, Node*> >, std::less<int>, std::allocator<std::pair<int const, Node*> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, Node*> const&)
 [113] RemoteConnection::RemoteConnection(Configuration const&) [72] std::_Deque_base<Message*, std::allocator<Message*> >::~_Deque_base() [92] std::_Rb_tree<int, std::pair<int const, Node*>, std::_Select1st<std::pair<int const, Node*> >, std::less<int>, std::allocator<std::pair<int const, Node*> > >::_M_insert_unique(std::pair<int const, Node*> const&)
  [39] Key::Key(long)         [65] std::_Deque_base<int, std::allocator<int> >::_M_create_nodes(int**, int**) [97] std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&)
   [6] Txn::writephase()      [68] std::_Deque_base<int, std::allocator<int> >::_M_destroy_nodes(int**, int**) [98] std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_unique(std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&)
  [54] Txn::messagephase()    [64] std::_Deque_base<int, std::allocator<int> >::_M_initialize_map(unsigned long) [99] std::_Rb_tree<int, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > >, std::_Select1st<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > >, std::pair<int const, std::vector<PreprocessorNode*, std::allocator<PreprocessorNode*> > > const&)
   [5] Txn::init()            [70] std::_Deque_base<int, std::allocator<int> >::~_Deque_base() [93] std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&)
  [18] Txn::read(Key, void*)  [75] std::deque<request, std::allocator<request> >::_M_pop_front_aux() [100] std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_unique(std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&)
  [73] Txn::begin(Worker*)    [74] std::deque<request, std::allocator<request> >::_M_push_back_aux(request const&) [94] std::_Rb_tree<int, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > >, std::_Select1st<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > >, std::pair<int const, std::vector<DBNode*, std::allocator<DBNode*> > > const&)
  [14] Txn::write(Key, void*) [77] std::deque<request, std::allocator<request> >::_M_reallocate_map(unsigned long, bool) [129] std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_(std::_Rb_tree_node_base const*, std::_Rb_tree_node_base const*, std::pair<int const, int> const&)
   [8] Txn::readphase()       [37] std::deque<request, std::allocator<request> >::erase(std::_Deque_iterator<request, request&, request*>) [130] std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_unique(std::pair<int const, int> const&)
  [46] Txn::Txn()             [49] std::deque<Txn*, std::allocator<Txn*> >::_M_pop_front_aux() [51] std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_insert_unique_(std::_Rb_tree_const_iterator<std::pair<int const, int> >, std::pair<int const, int> const&)
  [33] Txn::~Txn()            [76] std::deque<Txn*, std::allocator<Txn*> >::_M_push_back_aux(Txn* const&)
  [19] Lock::owners_unsafe(std::deque<Txn*, std::allocator<Txn*> >*) [88] std::deque<Txn*, std::allocator<Txn*> >::_M_reallocate_map(unsigned long, bool)
