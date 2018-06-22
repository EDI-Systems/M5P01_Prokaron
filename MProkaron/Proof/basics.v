(******************************************************************************
Filename    : basics.v
Author      : pry
Date        : 07/06/2018
Licence     : LGPL v3+; see COPYING for details.
Description : The basic lemmas for RMP's correctness proof. This is a coq file;
              it has nothing to do with verilog!
******************************************************************************)

(* Require Imports ***********************************************************)
Require Import PeanoNat.
Require Import Arith.

Local Open Scope nat_scope.
(* End Require Imports *******************************************************)

(* Require Exports ***********************************************************)

(* End Require Exports *******************************************************)

Definition nandb (b1 :bool) (b2 :bool) : bool
:=
    match b1 with
        |false => true
        |true =>
            match b2 with
                | true => false
                | false => true
            end
    end
.

Example test_nandb1: (nandb true false) = true.
Proof. simpl. reflexivity. Qed.
Example test_nandb2: (nandb false false) = true.
Proof. simpl. reflexivity. Qed.
Example test_nandb3: (nandb false true) = true.
Proof. simpl. reflexivity. Qed.
Example test_nandb4: (nandb true true) = false.
Proof. simpl. reflexivity. Qed.

Definition andb3 (b1:bool)(b2:bool)(b3:bool):bool
:=
    match b1 with 
        |false=>false
        |true=>(andb b2 b3)
    end
.
Example test_andb31:                 (andb3 true true true) = true.
Proof. reflexivity.  Qed.
Example test_andb32:                 (andb3 false true true) = false.
Proof. reflexivity.  Qed.
Example test_andb33:                 (andb3 true false true) = false.
Proof. reflexivity.  Qed.
Example test_andb34:                 (andb3 true true false) = false.
Proof. reflexivity.  Qed.

Check negb.
Check S.
Check O.

Fixpoint plus (n : nat) (m : nat) : nat :=
  match n with
    | O => m
    | S n' => S (plus n' m)
  end.

Fixpoint mult (n m : nat) : nat :=
  match n with
    | O => O
    | S n' => plus m (mult n' m)
  end.
(*Slightly different from standard fac*)
Fixpoint factorial (n:nat) : nat
:=
    match n with
    |O=>O
    |(S O)=>(S O)
    |(S n')=>mult (factorial n') (S n')
    end
.

Example test_factorial1:          (factorial 3) = 6.
Proof. reflexivity.  Qed.
Example test_factorial2:          (factorial 5) = (mult 10 12).
Proof. reflexivity.  Qed.
Example test_factorial3:          (factorial 0) = 0.
Proof. reflexivity.  Qed.

Theorem plus_O_n : forall n : nat, 0 + n = n.
Proof.
  intros n. simpl. reflexivity.  Qed.

Theorem plus_id_example : forall n m:nat,
  n = m ->
  n + n = m + m.

Proof.
  intros n m.   (* move both quantifiers into the context *)
  intros H.     (* move the hypothesis into the context *)
  rewrite <- H. (* Rewrite the goal using the hypothesis *)
  reflexivity.  Qed.

Theorem plus_id_exercise : forall n m o : nat,
  n = m -> m = o -> n + m = m + o.

Proof.
  intros n m o.
  intros H1.
  intros H2.
  rewrite <- H1.
  rewrite -> H1.
  rewrite -> H2.
  reflexivity.  Qed.

Theorem mult_0_plus : forall n m : nat,
  (0 + n) * m = n * m.
(*intros should be close to where they are used first*)
Proof.
  intros m.
  intros n.
  simpl.
  reflexivity. Qed.

Theorem mult_S_1 : forall n m : nat,
  m = S n ->
  m * (1 + n) = m * m.

Proof.
  intros m.
  intros n.
  intros H.
  rewrite -> H.
  simpl.
  reflexivity. Qed.

Fixpoint beq_nat (n m : nat) : bool :=
  match n with
  | O => match m with
	 | O => true
	 | S m' => false
	 end
  | S n' => match m with
	    | O => false
	    | S m' => beq_nat n' m'
	    end
  end.

Theorem plus_1_neq_0_firsttry : forall n : nat,
  beq_nat (n + 1) 0 = false.
Proof.
  intros n.
  destruct n as [|].
 -reflexivity.
 -reflexivity.
Qed.

Theorem identity_fn_applied_twice :
  forall (f : bool -> bool),
  (forall (x : bool), f x = x) ->
  forall (b : bool), f (f b) = b.

Proof.
  intros f.
  intros H.
  intros b.
  rewrite <- H.
  rewrite <- H.
  reflexivity.
Qed.

Theorem andb_eq_orb :
  forall (b c : bool),
  (andb b c = orb b c) ->
  b = c.


Proof.
  intros b.
  intros c.
  intros H.
  destruct b.
  -destruct c.
   +reflexivity.
   +discriminate.
  -destruct c.
   +discriminate.
   +reflexivity.
Qed.

Inductive mybin:Type:=
  | B : mybin
  | X : mybin -> mybin
  | Y : mybin -> mybin
.

Fixpoint incr (n:mybin):mybin:=
  match n with
  | B => (X B)
  | (X m) => (Y (X m))
  | (Y k) => (X (Y k))
  end
.

Fixpoint mybin2nat (n:mybin):nat:=
  match n with
  | B => O
  | (X B) => (S O)
  | (Y (X B)) => (S (S O))
  | (X m) => (1 + (mybin2nat m))
  | (Y n) => (2 * (mybin2nat n))
  end
.

Inductive bin : Type :=
  | Z : bin
  | T : bin -> bin
  | M : bin -> bin.

Fixpoint inc (b : bin) : bin :=
  match b with
  | Z    => M Z         (* 0      -> 1        *)
  | T b' => M b'        (* 2n     -> 2n + 1   *)
  | M b' => T (inc b')  (* 2n + 1 -> 2(n + 1) *)
  end.

Fixpoint bin2nat (b : bin) : nat :=
  match b with
  | Z    => O
  | T b' => (bin2nat b') * 2
  | M b' => S ((bin2nat b') * 2)
  end.

Lemma s_is_plus_one : forall n:nat, S n = n + 1.
Proof.
intros. induction n.
 - reflexivity.
 - simpl. rewrite <- IHn. reflexivity.
Qed.

Lemma p1m2ism2p1 : forall n:nat, (n*2)+2=(n+1)*2.
Proof.
  intros. induction n.
  trivial.
  simpl. rewrite <- IHn. reflexivity.
Qed.

Theorem whatsthis:
  forall (b:bin), (bin2nat b) + 1 = bin2nat (inc b)
.

Proof.
  intros b.
  induction b.
  trivial.
  -simpl.
   rewrite <- s_is_plus_one.
   reflexivity.
  -simpl.
   rewrite <- s_is_plus_one.
   rewrite <- IHb.
   rewrite <- s_is_plus_one.
   simpl.
   reflexivity.
Qed.
   
Fixpoint double (n:nat) :=
  match n with
  | O => O
  | S n' => S (S (double n'))
  end.

(** Use induction to prove this simple fact about [double]: *)

Fact sseq: forall (a b:nat), a = b -> S a = S b.
Proof.
  intros a b H.
  rewrite H.
  reflexivity.
Qed.

Lemma double_plus : forall n, double n = n + n .
Proof.
  intros n.
  induction n.
  trivial.
  simpl.
  rewrite -> IHn.
  apply sseq.
  rewrite -> s_is_plus_one.
  rewrite <- plus_assoc.
  rewrite <- s_is_plus_one.
  reflexivity.
Qed.
  
  
  

(* End Of File ***************************************************************)

(* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************)
