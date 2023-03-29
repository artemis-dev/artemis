ARTEMIS Example of Projection Definition File
=============================================
# Revision
0.1 March 07, 2014 by Shinsuke OTA 

# Overview

art::TreeProjectionProcessor can project the contents of
art::TEventCollections to 1-D, 2-D, or 3-D historgrams.
In this document the detail of definition file is described.

There are three main sections: include, alias and group.
The sections 'include' and 'alias' are optional.
The section 'group' is mandatory.

In the section "include" the definition files to be included are listed.
The definition files in include section are read before reading the
definitions in the parent file. The definitions in the parent file
does not affect the definisions in its children.
DO NOT INCLUDE its parent, grandparent, ... file.

The section "alias" lists the aliases to be used as variable or
selection.  One alias can be defined as a hash, i.e. the name of
alias is a key and the content is a value.  The alias can contains
the other alias.

The section "group" is the main section in the definition.
You can create several groups containing the 1-D, 2-D and 3-D projections.
Each projection can be cloned in the same group.
Each group can be cloned.
The cut inherits when group or projection is cloned.

# include section

# alias section

# group section

The section "group" should be a sequence. In each node 'name' and
'title' are mandatory, and 'contents' or 'clone' should be contained.
The available key and its meaning is listed below.

Normal group consists of the hash of *name*, *title* and *contents*. The *cut* is optional.

... under construction ...











## hogehoge
