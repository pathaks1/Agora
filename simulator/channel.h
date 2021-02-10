#ifndef SIM_CHAN_MODEL
#define SIM_CHAN_MODEL

#include <algorithm>
#include <armadillo>
#include <cassert>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <numeric>

#include "buffer.h"
#include "config.h"
#include "gettime.h"
#include "memory_manage.h"
#include "signal_handler.h"
#include "symbols.h"
#include "utils.h"

using namespace arma;

class Channel {
 public:
  Channel(Config* config_bs, Config* config_ue, std::string channel_type,
          double channel_snr);
  ~Channel();

  // Dimensions of fmat_src: ( bscfg->sampsPerSymbol, uecfg->UE_ANT_NUM )
  void ApplyChan(const cx_fmat& fmat_src, cx_fmat& mat_dst,
                 const bool is_downlink, const bool is_newChan);

  // Additive White Gaussian Noise. Dimensions of src: ( bscfg->sampsPerSymbol,
  // uecfg->UE_ANT_NUM )
  void Awgn(const cx_fmat& fmat_src, cx_fmat& fmat_dst) const;

  /*
   * From "Study on 3D-channel model for Elevation Beamforming
   *       and FD-MIMO studies for LTE"
   *       3GPP TR 36.873 V12.7.0 (2017-12)
   * Note: FD-MIMO Stands for Full-Dimension MIMO
   * Currenlty implements the 3D-UMa scenario only. This
   * corresponds to an Urban Macro cell with high UE density
   * indoor and outdoor. This scenario assumes Base Stations
   * are above surrounding buildings.
   *
   */
  void Lte3gpp(const cx_fmat& fmat_src, cx_fmat& fmat_dst);

 private:
  Config* bscfg_;
  Config* uecfg_;

  Channel* channel_;
  size_t bs_ant_;
  size_t ue_ant_;
  size_t n_samps_;

  std::string sim_chan_model_;
  double channel_snr_db_;
  enum ChanModel { kAwgn, kRayleigh, kRan3Gpp } chan_model_;

  cx_fmat h_;
};

#endif /* SIM_CHAN_MODEL */
