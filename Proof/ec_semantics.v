(* This file defines the C semantics of what is used in RMP.
  Different from a full-fledged C language specification, we make assumptions 
  that are made by the RMP, notably:
  1. Only two integer data types are present in addition to u8_t, ret_t and 
     ptr_t, where they must be at least as long as the logical machine word.
     A logical machine word is defined as a word length that is  
     (1) a power-of-2,
     (2) a multiple of u8_t,
     (3) long enough to contain all pointers be it any type on the platform.
  2. u8_t is at least 8 bits.
  
  We'll use the Coq stdlib in our proof, but we will define whatever datatype
  that appears in our program as our own.
  
  Bib: Software Foundations, vol.1
 *)
 
(* Import *)
Require Import Notations Logic Datatypes.
Require Import Arith.
Require Decimal Hexadecimal Number.
Local Open Scope nat_scope.
 
Module EC.

Lemma S_inj : forall (n m : nat),
  S n = S m ->
  n = m.
Proof.
  intros n m H1.
  assert (H2: n = pred (S n)).
  reflexivity.
  rewrite H2.
  rewrite H1.
  reflexivity.
Qed.


Lemma S_same : forall (n : nat),
  (n =? n) = true.
Proof.
  intros n.
  induction n.
  reflexivity.
  inversion IHn.
  simpl.
  reflexivity.
Qed.
  
Lemma S_inj_p : forall (n m : nat),
  (n =? m) = true -> n = m.
Proof.
  intros n.
  induction n.
  intros m.
  destruct m.
  trivial.
  simpl.
  discriminate.
  intros m IHm.
  destruct m.
  discriminate.
  apply f_equal.
  apply IHn.
  simpl in IHm.
  exact IHm.
Qed.

Check @eq_refl.
Check eq_refl 3.
Check eq_ind.

Print S_inj_p.
  
Theorem S_inj2 : forall (n m : nat) (b : bool),
  ((S n) =? (S m)) = b ->
  (n =? m) = b.
Proof.
  intros n m b H. 
  destruct b.
  simpl in H.
  apply H.
  simpl in H.
  apply H.
Qed.

(* Bit definition - a single bit is either true or false *)
Inductive Bit:Type:=
  | True
  | False.
  
(* Pack of bits definition - a list of bits is a bitpack *)
Inductive Pack:Type:=
  | nil
  | cons(b:Bit)(p:Pack).

(* Nice notations to make the bitlist easier to write *)
Notation "b :: p":=(cons b p).
Notation "[ ]" := nil.
Notation "[ b1 ; .. ; bn ]":=(cons b1 .. (cons bn nil) ..).

(* Check if we have defined it correctly. *)
Check False.
Check [True;False;True;False].

(* Compute length of a certain bitlist *)
Fixpoint Pack_Len(p:Pack):nat:=
  match p with
  | nil => O
  | b :: p => S (Pack_Len p)
  end.
  
Compute Pack_Len([True;False;True;False]).

(* Reversed binary numbers *)
Inductive Revbin:Type:=
  | R
  | R0 : Revbin -> Revbin
  | R1 : Revbin -> Revbin.

(* Extract a certain bit in the bitlist *)
Fixpoint Revbin_Bit_Get (r:Revbin) (n:nat):Bit:=
  match n with
  | O =>
    match r with
    |  R => False
    |  R0 r' => False
    |  R1 r' => True
    end
  | S n' =>
    match r with
    |  R => False
    |  R0 r' => Revbin_Bit_Get (r') (n')
    |  R1 r' => Revbin_Bit_Get (r') (n')
    end
  end.
  
Fixpoint Revbin_Len_Get (r:Revbin):nat:=
  match r with
  | R => O
  | R0 r' => S (Revbin_Len_Get (r'))
  | R1 r' => S (Revbin_Len_Get (r'))
  end.
  
Compute Revbin_Len_Get(R1 (R1 (R0 (R1 R)))).

Fixpoint Revbin_Incr (r:Revbin):Revbin:=
  match r with
  | R => R1 R
  | R0 r' => R1 r'
  | R1 r' => R0 (Revbin_Incr (r'))
  end.
  
Compute Revbin_Incr(R1 (R1 (R1 (R1 R)))).

Fixpoint Revbin_Decr (r:Revbin):Revbin:=
  match r with
  | R => R
  | R1 r' => R0 r'
  | R0 r' => R1 (Revbin_Decr (r'))
  end.

Compute Revbin_Decr(R0 (R0 (R0 (R1 R)))).


Fixpoint Revbin_To_Nat (r:Revbin):nat:=
  match r with
  | R => O
  | R0 r' => Revbin_To_Nat(r') + Revbin_To_Nat(r')
  | R1 r' => S (Revbin_To_Nat(r') + Revbin_To_Nat(r'))
  end.

Compute Revbin_To_Nat(R1 (R1 (R0 (R1 R)))).
Fixpoint Nat_To_Revbin (n:nat):Revbin:=
  match n with
  | O => R
  | S n' => Revbin_Incr(Nat_To_Revbin(n'))
  end.
  
Compute Nat_To_Revbin(256).

Theorem Rev_Nat_Incr_Commute: forall b:Revbin, Revbin_To_Nat(Revbin_Incr b) = S (Revbin_To_Nat b).
Proof.
intros b.
induction b.
reflexivity.
unfold Revbin_Incr.
unfold Revbin_To_Nat.
reflexivity.
simpl.
rewrite -> IHb.
ring.
Qed.

Theorem Nat_Rev_Incr_Commute: forall n:nat, Nat_To_Revbin(S n) = Revbin_Incr (Nat_To_Revbin n).
Proof.
intros n.
unfold Nat_To_Revbin.
reflexivity.
Qed.

(* Converting revbin to nat then back is the same *)
Theorem Rev_Nat_Rev_Same: forall n, Revbin_To_Nat (Nat_To_Revbin n) = n.
Proof.
intros n.
induction n.
unfold Revbin_To_Nat.
unfold Nat_To_Revbin.
reflexivity.
rewrite -> Nat_Rev_Incr_Commute.
rewrite -> Rev_Nat_Incr_Commute.
rewrite -> IHn.
reflexivity.
Qed.

(* Need to define computations on GF28, GF232. or, just an abstract group GF2X.
 * also define their representations in natural numbers, so we can possibly cling to nat for computation, and just
 * convert things back when we need to.*)
 
(* Ref:Correct-by-Construction Finite Field Arithmetic in Coq *)

(* Need to define the data types BYTE, WORD, DPTR, IPTR. IPTR could be left out. *)

(* Let's start from a byte (GF28), and carry on from there. *)

(* Rank definition - only four ranks exist, where Iptr cannot be compared w/anything else *)
Inductive Rank:Type:=
  | Byte
  | Word
  | Dptr
  | Iptr.

(* Signedness definition *)
Inductive Signedness:Type:=
  | Signed
  | Unsigned.
  
(* Bitlist definition *)


(* u8_t definition *)
Inductive U8_Type:Type:=
  | m: nat.


(* ptr_t definition *)


(* ret_t definition *)


(* cnt_t definition - cnt_t is just a nickname of ret_t *)

End EC.
